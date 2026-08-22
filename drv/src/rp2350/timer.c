#include "timer.h"

void timer_enable()
{
    *TIMER_SIO_REG_MTIME_CTRL |= 1u | (1u << 1);
}

void timer_disable()
{
    *TIMER_SIO_REG_MTIME_CTRL &= ~1u;
}

bool timer_is_enabled()
{
    return (*TIMER_SIO_REG_MTIME_CTRL & 1) == 1;
}

uint64_t timer_get_current()
{
    return ((uint64_t)*TIMER_SIO_REG_MTIMEH << 32) | *TIMER_SIO_REG_MTIME;
}