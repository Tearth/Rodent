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
    return (*TIMER_SIO_REG_MTIME_CTRL & 1) != 0;
}

uint64_t timer_get_current()
{
    uint64_t mtimeh;
    uint64_t mtime;

    do
    {
        // Read MTIMEH
        mtimeh = *TIMER_SIO_REG_MTIMEH;

        // Read MTIME
        mtime = *TIMER_SIO_REG_MTIME;
    }
    while (mtimeh != *TIMER_SIO_REG_MTIMEH);

    return ((uint64_t)mtimeh << 32) | mtime;
}

void timer_set_comparator(uint64_t value)
{
    // Set MTIMECMP to a value guaranteeing no accidental interrupt
    *TIMER_SIO_REG_MTIMECMP = UINT32_MAX;

    // Set MTIMECMPH
    *TIMER_SIO_REG_MTIMECMPH = (uint32_t)(value >> 32);

    // Set MTIMECMP
    *TIMER_SIO_REG_MTIMECMP = (uint32_t)value;
}