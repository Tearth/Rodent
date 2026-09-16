#include "sched.h"

static proc_t procs[MAX_PROCS] = {};
static uint8_t current_pid = UINT8_MAX;

static void sched_next();
static uint64_t sched_duration_to_deadline(const uint32_t duration);

void sched_init(const boot_proc_t *boot_procs)
{
    arch_attach_timer_handler(sched_timer_handler);

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

    log_msg(LOG_LEVEL_OK, "Initialized scheduler");
}

void sched_run()
{
    log_msg(LOG_LEVEL_INFO, "Entering uspace");

    if (procs[0].status == PROC_STATUS_IDLE)
    {
        mcu_systime_set_comparator(sched_duration_to_deadline(20));
        uspace_enter(&procs[0].regs);
    }

    log_msg(LOG_LEVEL_FAIL, "Failed to run scheduler, no process available");
}

void sched_timer_handler(regs_t *regs)
{
    if (current_pid != UINT8_MAX)
    {
        procs[current_pid].status = PROC_STATUS_IDLE;
        memcpy(&procs[current_pid].regs, regs, sizeof(regs_t));
    }

    sched_next();
}

void sched_sleep(regs_t *regs, const uint32_t duration)
{
    procs[current_pid].status = PROC_STATUS_SLEEPING;
    procs[current_pid].deadline = sched_duration_to_deadline(duration);
    memcpy(&procs[current_pid].regs, regs, sizeof(regs_t));

    sched_next();
}

static void sched_next()
{
    const uint64_t systime = mcu_systime_get_current();

    uint64_t deadline = UINT64_MAX;
    uint8_t next_pid = UINT8_MAX;

    // Find any sleeping thread with expired deadline
    for (size_t i = 0; i <= MAX_PROCS; i++)
    {
        const size_t pid = (current_pid + i + 1) % MAX_PROCS;

        if (procs[pid].status == PROC_STATUS_SLEEPING)
        {
            if (procs[pid].deadline <= systime)
            {
                next_pid = pid;
            }
            else if (procs[pid].deadline < deadline)
            {
                deadline = procs[pid].deadline;
            }
        }
    }

    if (next_pid == UINT8_MAX)
    {
        // Find any idle thread ready to run
        for (size_t i = 0; i <= MAX_PROCS; i++)
        {
            const size_t pid = (current_pid + i + 1) % MAX_PROCS;

            if (procs[pid].status == PROC_STATUS_IDLE)
            {
                next_pid = pid;
                break;
            }
        }
    }

    if (deadline == UINT64_MAX)
    {
        deadline = sched_duration_to_deadline(20);
    }

    mcu_systime_set_comparator(deadline);

    if (next_pid != UINT8_MAX)
    {
        procs[next_pid].status = PROC_STATUS_RUNNING;
        current_pid = next_pid;

        uspace_enter(&procs[next_pid].regs);
    }

    // No available thread was found, wait for the next interrupt
    current_pid = UINT8_MAX;

    while (1)
    {
        arch_irq_enable();
        arch_irq_wait();
    }
}

static uint64_t sched_duration_to_deadline(const uint32_t duration)
{
    const uint64_t systime = mcu_systime_get_current();
    const uint64_t freq = mcu_sysclk_get_freq();
    const uint64_t delta = duration * freq / 1000;

    return systime + delta;
}