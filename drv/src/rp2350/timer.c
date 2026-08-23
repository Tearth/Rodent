#include "timer.h"

void timer_enable()
{
    // Set EN (Timer Enable), FULLSPEED (Increment Every Cycle)
    *TIMER_SIO_REG_MTIME_CTRL |= 1u | (1u << 1);
}

void timer_disable()
{
    // Clear EN (Timer Enable)
    *TIMER_SIO_REG_MTIME_CTRL &= ~1u;
}

bool timer_is_enabled()
{
    // Read EN (Timer Enable)
    return (*TIMER_SIO_REG_MTIME_CTRL & 1) == 1;
}

uint64_t timer_get_current()
{
    // Read MTIMEH
    uint32_t mtimeh = (uint64_t)*TIMER_SIO_REG_MTIMEH << 32;

    // Read MTIME
    uint32_t mtime = *TIMER_SIO_REG_MTIME;

    return mtimeh | mtime;
}

void timer_set_comparator(uint64_t value)
{
    // Set MTIMECMPH
    *TIMER_SIO_REG_MTIMECMPH = (uint32_t)(value >> 32);

    // Set MTIMECMP
    *TIMER_SIO_REG_MTIMECMP = (uint32_t)value;
}