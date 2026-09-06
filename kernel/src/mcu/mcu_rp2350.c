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

    utoa(systime_get_current(), buf, 10);
    log_msg(LOG_LEVEL_OK, "Started system time");
    log_fmt(LOG_LEVEL_INFO, " Now @ ", buf, " ticks", nullptr);

    return true;
}

uint64_t systime_get_current()
{
    return timer_get_current();
}