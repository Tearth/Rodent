#include <stdlib.h>
#include <shared/boot.h>
#include "arch.h"
#include "def.h"
#include "log.h"
#include "mcu.h"
#include "sched.h"
#include "syscall.h"

mutex_t kmain_mutex = {};
uint32_t cores_to_init = CPU_CORES;

static void core_finish_init();
static void core_wait_for_all();

[[noreturn]] void kmain(boot_args_t *boot_args)
{
    log_msg(LOG_LEVEL_OK, "Rodent Kernel");

    arch_mutex_lock(&kmain_mutex);
    {
        if (!arch_init(true))
        {
            HALT();
        }

        if (!mcu_init(true))
        {
            HALT();
        }

        syscall_init();
        sched_init(boot_args->procs);
    }
    arch_mutex_unlock(&kmain_mutex);

    arch_irq_enable();
    core_finish_init();
    core_wait_for_all();

    log_msg(LOG_LEVEL_INFO, "Enabling scheduler");

    sched_enable();
    sched_run();
}

[[noreturn]] void kmain_core()
{
    arch_mutex_lock(&kmain_mutex);
    {
        if (!arch_init(false))
        {
            HALT();
        }

        if (!mcu_init(false))
        {
            HALT();
        }
    }
    arch_mutex_unlock(&kmain_mutex);

    arch_irq_enable();
    core_finish_init();
    core_wait_for_all();
    sched_run();
}

static void core_finish_init()
{
    cores_to_init--;
}

static void core_wait_for_all()
{
    while (cores_to_init > 0)
    {
        NOP();
    }
}