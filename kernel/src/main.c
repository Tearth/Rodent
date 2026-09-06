#include <stdlib.h>
#include "arch/arch.h"
#include "shared/boot.h"
#include "mcu/mcu.h"
#include "log.h"

static void halt();

int kmain(boot_iface_t *boot_iface, boot_args_t *boot_args)
{
    log_init(boot_iface);
    log_msg(LOG_LEVEL_OK, "Rodent Kernel");

    if (!arch_init())
    {
        log_msg(LOG_LEVEL_FAIL, "Failed to init CPU");
        halt();
    }

    log_msg(LOG_LEVEL_OK, "Finished CPU initialization");

    if (!mcu_init())
    {
        log_msg(LOG_LEVEL_FAIL, "Failed to init MCU");
        halt();
    }

    log_msg(LOG_LEVEL_OK, "Finished MCU initialization");

    while (1);
}

static void halt()
{
    while (1);
}