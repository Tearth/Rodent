#include "hal.h"
#include <rp2350/timer.h>
#include <rp2350/uart.h>

void hal_uart_send_str(const char *str)
{
    uart_send_str(UART0, str);
}

bool hal_systime_init()
{
    return timer_sys_enable(), true;
}

uint64_t hal_systime_get_current()
{
    return timer_sys_get_current();
}