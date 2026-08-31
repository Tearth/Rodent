#include <stdlib.h>
#include "arch/irq_arch.h"
#include "shared/boot.h"
#include "mcu/mcu.h"
#include "log.h"

static bool init_systime();
static bool init_irq();
static void halt();

int kmain(boot_iface_t *boot_iface, boot_args_t *boot_args)
{
    log_init(boot_iface);
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

static bool init_systime()
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

static bool init_irq()
{
    if (!irq_enable())
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to init interrupts"), false;
    }

    log_msg(LOG_LEVEL_OK, "Initialized interrupts");

    return true;
}

static void halt()
{
    while (1);
}