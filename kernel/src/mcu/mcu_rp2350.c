#include "mcu_rp2350.h"

void uart_send(const char *str)
{
    uart_send_str(UART0, str);
}

bool systime_init()
{
    return timer_enable(), true;
}

uint64_t systime_get_current()
{
    return timer_get_current();
}