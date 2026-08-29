#include "mcu_rp2350.h"

bool systime_init()
{
    return timer_enable(), true;
}

uint64_t systime_get_current()
{
    return timer_get_current();
}