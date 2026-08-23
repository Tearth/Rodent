#include <stdlib.h>
#include "cpu/cpu.h"
#include "log.h"
#include "arch/irq_arch.h"

static bool init_systime();
static bool init_irq();
static void halt();

int main()
{
    log_msg(LOG_LEVEL_OK, "Rodent Kernel");

    if (!init_systime())
    {
        halt();
    }

    if (!init_irq())
    {
        halt();
    }

    while (1);
}

bool init_systime()
{
    char buf[16];

    if (!systime_init())
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to init system time"), false;
    }

    utoa(systime_get_current(), buf, 10);
    log_msg(LOG_LEVEL_OK, "Started system time");
    log_fmt(LOG_LEVEL_INFO, " Now @ ", buf, " ticks", nullptr);

    return true;
}

bool init_irq()
{
    if (!irq_enable())
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to init interrupts"), false;
    }

    log_msg(LOG_LEVEL_OK, "Initialized interrupts");

    return true;
}

void halt()
{
    while (1);
}