#include <stdlib.h>
#include "arch/arch.h"
#include "shared/boot.h"
#include "shared/halt.h"
#include "mcu/mcu.h"
#include "log.h"
#include "sched.h"

int kmain(boot_iface_t *boot_iface, boot_args_t *boot_args)
{
    log_init(boot_iface);
    log_msg(LOG_LEVEL_OK, "Rodent Kernel");

    if (!arch_init())
    {
        log_msg(LOG_LEVEL_FAIL, "Failed to init CPU");
        HALT();
    }

    log_msg(LOG_LEVEL_OK, "Finished CPU initialization");

    if (!mcu_init())
    {
        log_msg(LOG_LEVEL_FAIL, "Failed to init MCU");
        HALT();
    }

    log_msg(LOG_LEVEL_OK, "Finished MCU initialization");

    arch_irq_enable();
    log_msg(LOG_LEVEL_OK, "Enabled interrupts");

    sched_init(boot_args->procs);
    log_msg(LOG_LEVEL_OK, "Initialized scheduler");

    log_msg(LOG_LEVEL_INFO, "Entering uspace");
    sched_run();

    HALT();
}