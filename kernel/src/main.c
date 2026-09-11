#include <stdlib.h>
#include "arch/arch.h"
#include "shared/boot.h"
#include "shared/halt.h"
#include "mcu/mcu.h"
#include "log.h"
#include "sched.h"
#include "syscall/syscall.h"

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