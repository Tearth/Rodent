#include "mcu_rp2350.h"

static bool mcu_init_systime();

bool mcu_init()
{
    if (!mcu_init_systime())
    {
        return false;
    }

    return true;
}

static bool mcu_init_systime()
{
    char buf[16];

    timer_enable();
    timer_set_comparator(UINT64_MAX);

    utoa(mcu_systime_get_current(), buf, 10);
    log_msg(LOG_LEVEL_OK, "Started system time");
    log_fmt(LOG_LEVEL_INFO, " Now @ ", buf, " ticks", EOL);

    return true;
}

uint32_t mcu_sysclk_get_freq()
{
    return clk_get_freq(CLK_SYS);
}

uint64_t mcu_systime_get_current()
{
    return timer_get_current();
}

void mcu_systime_set_comparator(uint64_t value)
{
    timer_set_comparator(value);
}