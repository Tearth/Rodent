#include "sched.h"

static proc_t procs[MAX_PROCS] = {};
static thread_t threads[MAX_THREADS] = {};
static uint8_t current_pid = UINT8_MAX;
static uint8_t current_tid = UINT8_MAX;

static void sched_next();
static uint64_t sched_duration_to_deadline(const uint32_t duration);

void sched_init(const boot_proc_t *boot_procs)
{
    arch_attach_timer_handler(sched_timer_handler);

    for (size_t i = 0; i < MAX_BOOT_THREADS; i++)
    {
        if (boot_procs[i].type == BOOT_PROC_TYPE_NONE)
        {
            continue;
        }

        memcpy(procs[i].path, boot_procs[i].path, MAX_PATH_LEN);

        procs[i].status = PROC_STATUS_RUNNING;
        procs[i].base = boot_procs[i].base;
        procs[i].entry = boot_procs[i].entry;
        procs[i].size = boot_procs[i].size;

        threads[i].pid = i;
        threads[i].status = THREAD_STATUS_IDLE;
        threads[i].priority = MIN_PRIORITY;
        threads[i].regs.pc = (uint32_t)boot_procs[i].entry;
    }

    log_msg(LOG_LEVEL_OK, "Initialized scheduler");
}

void sched_run()
{
    log_msg(LOG_LEVEL_INFO, "Entering uspace");

    if (threads[0].status == THREAD_STATUS_IDLE)
    {
        mcu_systime_set_comparator(sched_duration_to_deadline(20));
        uspace_enter(&threads[0].regs);
    }

    log_msg(LOG_LEVEL_FAIL, "Failed to run scheduler, no process available");
}

void sched_timer_handler(regs_t *regs)
{
    if (current_tid != UINT8_MAX)
    {
        threads[current_tid].status = THREAD_STATUS_IDLE;
        memcpy(&threads[current_tid].regs, regs, sizeof(regs_t));
    }

    sched_next();
}

void sched_sleep(regs_t *regs, const uint32_t duration)
{
    threads[current_tid].status = THREAD_STATUS_SLEEPING;
    threads[current_tid].deadline = sched_duration_to_deadline(duration);
    memcpy(&threads[current_tid].regs, regs, sizeof(regs_t));

    sched_next();
}

uint8_t sched_get_current_pid()
{
    return current_pid;
}

uint8_t sched_get_current_tid()
{
    return current_tid;
}

uint8_t sched_get_priority(const uint8_t tid)
{
    if (threads[tid].status == THREAD_STATUS_NONE)
    {
        return UINT8_MAX;
    }

    return threads[tid].priority;
}

bool sched_set_priority(const uint8_t tid, const uint8_t priority)
{
    if (threads[tid].status == THREAD_STATUS_NONE)
    {
        return false;
    }

    if (priority < MIN_PRIORITY || priority > MAX_PRIORITY)
    {
        return false;
    }

    return threads[tid].priority = priority, true;
}

static void sched_next()
{
    const uint64_t systime = mcu_systime_get_current();

    uint64_t deadline = UINT64_MAX;
    uint8_t next_tid = UINT8_MAX;

    for (uint8_t p = MAX_PRIORITY; p >= MIN_PRIORITY && next_tid == UINT8_MAX; p--)
    {
        // Find any sleeping thread with expired deadline
        for (size_t i = 0; i <= MAX_THREADS; i++)
        {
            const size_t tid = (current_tid + i + 1) % MAX_THREADS;

            if (threads[tid].priority != p)
            {
                continue;
            }

            if (threads[tid].status == THREAD_STATUS_SLEEPING)
            {
                if (threads[tid].deadline <= systime)
                {
                    next_tid = tid;
                }
                else if (threads[tid].deadline < deadline)
                {
                    deadline = threads[tid].deadline;
                }
            }
        }

        if (next_tid == UINT8_MAX)
        {
            // Find any idle thread ready to run
            for (size_t i = 0; i <= MAX_THREADS; i++)
            {
                const size_t tid = (current_tid + i + 1) % MAX_THREADS;

                if (threads[tid].priority != p)
                {
                    continue;
                }

                if (threads[tid].status == THREAD_STATUS_IDLE)
                {
                    next_tid = tid;
                    break;
                }
            }
        }
    }

    if (deadline == UINT64_MAX)
    {
        deadline = sched_duration_to_deadline(20);
    }

    mcu_systime_set_comparator(deadline);

    if (next_tid != UINT8_MAX)
    {
        threads[next_tid].status = THREAD_STATUS_RUNNING;
        current_pid = threads[next_tid].pid;
        current_tid = next_tid;

        uspace_enter(&threads[next_tid].regs);
    }

    // No available thread was found, wait for the next interrupt
    current_pid = UINT8_MAX;
    current_tid = UINT8_MAX;

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