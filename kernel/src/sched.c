#include "sched.h"

static proc_t procs[MAX_PROCS] = {};
static uint8_t current_pid = 0;

static void sched_timer_arm_delay(uint32_t delay);
static void sched_timer_arm_deadline(uint64_t deadline);

void sched_init(boot_proc_t *boot_procs)
{
    for (size_t i = 0; i < MAX_BOOT_PROCS; i++)
    {
        if (boot_procs[i].type == BOOT_PROC_TYPE_NONE)
        {
            continue;
        }

        memcpy(procs[i].path, boot_procs[i].path, MAX_PATH_LEN);

        procs[i].base = boot_procs[i].base;
        procs[i].entry = boot_procs[i].entry;
        procs[i].size = boot_procs[i].size;
        procs[i].status = PROC_STATUS_IDLE;
        procs[i].regs.pc = (uint32_t)boot_procs[i].entry;
    }

    arch_attach_timer_handler(sched_timer_handler);
}

void sched_run()
{
    if (procs[0].status == PROC_STATUS_IDLE)
    {
        sched_timer_arm_delay(20);
        uspace_enter(&procs[0].regs);
    }

    log_msg(LOG_LEVEL_FAIL, "Failed to run scheduler, no process available");
}

void sched_timer_handler(regs_t *regs)
{
    procs[current_pid].status = PROC_STATUS_IDLE;
    memcpy(&procs[current_pid].regs, regs, sizeof(regs_t));

    sched_timer_arm_delay(20);

    for (size_t i = 0; i < MAX_PROCS; i++)
    {
        size_t pid = (current_pid + i + 1) % MAX_PROCS;
        if (procs[pid].status == PROC_STATUS_IDLE)
        {
            procs[pid].status = PROC_STATUS_RUNNING;
            current_pid = pid;

            uspace_enter(&procs[pid].regs);
        }
    }
}

static void sched_timer_arm_delay(uint32_t delay_ms)
{
    uint64_t systime = mcu_systime_get_current();
    uint64_t freq = mcu_sysclk_get_freq();
    uint64_t delta = delay_ms * freq / 1000;

    mcu_systime_set_comparator(systime + delta);
}

static void sched_timer_arm_deadline(uint64_t deadline)
{
    mcu_systime_set_comparator(deadline);
}