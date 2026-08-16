#include "timer.h"

void timer_sys_enable()
{
    *TIMER_SIO_REG_MTIME_CTRL |= 1u | (1u << 1);
}

void timer_sys_disable()
{
    *TIMER_SIO_REG_MTIME_CTRL &= ~1u;
}

bool timer_sys_is_enabled()
{
    return (*TIMER_SIO_REG_MTIME_CTRL & 1) == 1;
}

uint64_t timer_sys_get_current()
{
    return ((uint64_t)*TIMER_SIO_REG_MTIMEH << 32) | *TIMER_SIO_REG_MTIME;
}