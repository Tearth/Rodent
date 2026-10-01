#include "rp2350.h"

extern void kmain_core();
extern void _irq_handler_entry();
static void mcu_init_systime();
static void mcu_init_timer_comparator();
static bool mcu_init_cores();

extern uint32_t __stack_pointer;

bool mcu_init(const bool core0)
{
    if (core0)
    {
        mcu_init_systime();
    }

    mcu_init_timer_comparator();

    if (core0)
    {
        if (!mcu_init_cores())
        {
            return false;
        }
    }

    return true;
}

static void mcu_init_systime()
{
    char buf[16];

    timer_enable();
    utoa(mcu_systime_get_current(), buf, 10);
    log_msg(LOG_LEVEL_OK, "Started system time");
    log_fmt(LOG_LEVEL_INFO, " Now @ ", buf, " ticks", EOL);
}

static void mcu_init_timer_comparator()
{
    timer_set_comparator(UINT64_MAX);
}

static bool mcu_init_cores()
{
    const uint32_t data[] = {
        0, 0, 1,
        (uint32_t)_irq_handler_entry,
        (uint32_t)&__stack_pointer - STACK_SIZE,
        (uint32_t)kmain_core
    };

    for (size_t t = 0; t < 10; t++)
    {
        bool success = true;

        for (size_t i = 0; i < LEN(data); i++)
        {
            if (data[i] == 0)
            {
                // Drain FIFO before sending data
                while (fifo_can_read())
                {
                    fifo_read();
                }

                ext_hazard3_unblock();
            }

            fifo_write(data[i]);
            ext_hazard3_unblock();

            // Restart procedure if sent value was not echoed
            if (data[i] != fifo_read())
            {
                success = false;
                break;
            }
        }

        if (success)
        {
            return true;
        }
    }

    return false;
}

uint32_t mcu_sysclk_get_freq()
{
    return clk_get_freq(CLK_SYS);
}

uint64_t mcu_systime_get_current()
{
    return timer_get_current();
}

void mcu_systime_set_comparator(const uint64_t value)
{
    timer_set_comparator(value);
}

void uart_send(const char *str)
{
    uart_send_str(UART0, str);
}