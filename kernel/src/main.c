#include <stdlib.h>
#include <shared/boot.h>
#include <shared/macro.h>
#include "arch.h"
#include "log.h"
#include "mcu.h"
#include "sched.h"
#include "syscall.h"

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
    syscall_init();
    sched_init(boot_args->procs);
    sched_run();

    HALT();
}