#include "irq.h"

bool arch_init()
{
    if (!irq_enable())
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to init interrupts"), false;
    }

    log_msg(LOG_LEVEL_OK, "Initialized interrupts");

    return true;
}