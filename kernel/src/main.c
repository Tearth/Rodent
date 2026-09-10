#include <stdlib.h>
#include "arch/arch.h"
#include "shared/boot.h"
#include "shared/halt.h"
#include "mcu/mcu.h"
#include "log.h"
#include "sched.h"

__attribute__((noreturn)) int kmain(boot_args_t *boot_args)
{
    log_msg(LOG_LEVEL_OK, "Rodent Kernel");

    if (!arch_init())
    {
        HALT();
    }

    if (!mcu_init())
    {
        HALT();
    }

    arch_irq_enable();
    log_msg(LOG_LEVEL_OK, "Enabled interrupts");

    sched_init(boot_args->procs);
    log_msg(LOG_LEVEL_OK, "Initialized scheduler");

    log_msg(LOG_LEVEL_INFO, "Entering uspace");
    sched_run();

    HALT();
}