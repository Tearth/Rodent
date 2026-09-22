#include "sched.h"

static proc_t procs[MAX_PROCS] = {};
static thread_t threads[MAX_THREADS] = {};
static uint8_t current_pid = UINT8_MAX;
static uint8_t current_tid = UINT8_MAX;

static void sched_next();
static void sched_save_thread(const regs_t *regs);
static uint64_t sched_duration_to_systime(const uint32_t duration);

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
        threads[i].regs.pc = (uint32_t)boot_procs[i].entry;
        threads[i].status = THREAD_STATUS_READY;
        threads[i].priority = MAX_PRIORITY;
        threads[i].awake_time = 0;
        threads[i].start_time = 0;
        threads[i].exit_time = 0;
        threads[i].budget = 0;
        threads[i].deadline = 0;
        threads[i].replenishment = 0;

        threads[i].sched_policy = SCHED_POLICY_ROUND_ROBIN;
        threads[i].sched_params.round_robin.slice = DEFAULT_SCHED_SLICE;
        threads[i].sched_params.round_robin.priority = MAX_PRIORITY;
    }

    log_msg(LOG_LEVEL_OK, "Initialized scheduler");
}

void sched_run()
{
    log_msg(LOG_LEVEL_INFO, "Entering uspace");
    sched_next();
}

void sched_timer_handler(regs_t *regs)
{
    if (current_tid != UINT8_MAX)
    {
        threads[current_tid].status = THREAD_STATUS_READY;
        sched_save_thread(regs);
    }

    sched_next();
}

void sched_sleep(regs_t *regs, const uint32_t duration)
{
    const uint64_t systime = mcu_systime_get_current();

    threads[current_tid].status = THREAD_STATUS_WAITING;
    threads[current_tid].awake_time = systime + sched_duration_to_systime(duration);

    sched_save_thread(regs);
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

sched_policy_t sched_get_policy(const uint8_t tid)
{
    if (tid >= MAX_THREADS)
    {
        return SCHED_POLICY_INVALID;
    }

    return threads[tid].sched_policy;
}

bool sched_set_policy(const uint8_t tid, const sched_policy_t policy)
{
    if (tid >= MAX_THREADS)
    {
        return false;
    }

    threads[tid].sched_policy = policy;

    return true;
}

bool sched_get_params(const uint8_t tid, sched_params_t *params)
{
    if (tid >= MAX_THREADS)
    {
        return false;
    }

    return *params = threads[tid].sched_params, true;
}

bool sched_set_params(const uint8_t tid, const sched_params_t *params)
{
    if (tid >= MAX_THREADS)
    {
        return false;
    }

    const uint64_t systime = mcu_systime_get_current();

    switch (threads[tid].sched_policy)
    {
        case SCHED_POLICY_ROUND_ROBIN:
        {
            threads[tid].priority = params->round_robin.priority;
            break;
        }
        case SCHED_POLICY_REAL_TIME:
        {
            threads[tid].priority = params->real_time.priority_high;
            threads[tid].budget = params->real_time.budget;
            threads[tid].deadline = systime + sched_duration_to_systime(params->real_time.deadline);
            threads[tid].replenishment = systime + sched_duration_to_systime(params->real_time.period);
            break;
        }
        default: return false;
    }

    return threads[tid].sched_params = *params, true;
}

static void sched_next()
{
    const uint64_t systime = mcu_systime_get_current();

    uint8_t next_tid = UINT8_MAX;
    uint64_t next_irq = UINT64_MAX;

    // Wake up threads from sleeping
    for (size_t tid = 0; tid < MAX_THREADS; tid++)
    {
        if (threads[tid].status == THREAD_STATUS_WAITING)
        {
            if (systime >= threads[tid].awake_time)
            {
                threads[tid].status = THREAD_STATUS_READY;
            }
            else
            {
                next_irq = MIN(next_irq, threads[tid].awake_time);
            }
        }

        if (threads[tid].status == THREAD_STATUS_READY && threads[tid].sched_policy == SCHED_POLICY_REAL_TIME)
        {
            // Clear budget of expired threads, so it can be properly deprioritized later
            if (systime >= threads[tid].deadline)
            {
                threads[tid].budget = 0;
            }

            // Replenish budget, update deadline and increase priority
            if (systime >= threads[tid].replenishment)
            {
                threads[tid].priority = threads[tid].sched_params.real_time.priority_high;
                threads[tid].budget = sched_duration_to_systime(threads[tid].sched_params.real_time.budget);
                threads[tid].deadline += sched_duration_to_systime(threads[tid].sched_params.real_time.deadline);
                threads[tid].replenishment += sched_duration_to_systime(threads[tid].sched_params.real_time.period);
                next_irq = MIN(next_irq, threads[tid].replenishment);
            }

            // Decrease priority when budget has been used
            if (threads[tid].budget == 0)
            {
                threads[tid].priority = threads[tid].sched_params.real_time.priority_low;
            }
        }
    }

    for (uint8_t p = MAX_PRIORITY; p >= MIN_PRIORITY && next_tid == UINT8_MAX; p--)
    {
        // Find any thread with real time policy that is ready to run
        for (size_t tid = 0; tid < MAX_THREADS; tid++)
        {
            if (threads[tid].priority != p)
            {
                continue;
            }

            if (threads[tid].status == THREAD_STATUS_READY && threads[tid].sched_policy == SCHED_POLICY_REAL_TIME)
            {
                // Find thread with the earliest deadline
                if (threads[tid].budget > 0)
                {
                    if (threads[tid].deadline < next_irq)
                    {
                        next_tid = tid;
                        next_irq = threads[tid].deadline;
                    }

                    next_irq = MIN(next_irq, threads[tid].deadline);
                    next_irq = MIN(next_irq, systime + threads[tid].budget);
                }
            }
        }

        if (next_tid == UINT8_MAX)
        {
            uint64_t exit_time = UINT64_MAX;

            // Find any idle thread ready to run
            for (size_t tid = 0; tid < MAX_THREADS; tid++)
            {
                if (threads[tid].priority != p)
                {
                    continue;
                }

                if (threads[tid].status == THREAD_STATUS_READY)
                {
                    if (threads[tid].exit_time < exit_time)
                    {
                        next_tid = tid;
                        exit_time = threads[tid].exit_time;
                    }
                }
            }
        }
    }

    if (next_tid != UINT8_MAX)
    {
        uint64_t slice;

        switch (threads[next_tid].sched_policy)
        {
            case SCHED_POLICY_ROUND_ROBIN:
            {
                slice = threads[next_tid].sched_params.round_robin.slice;
                break;
            }
            case SCHED_POLICY_REAL_TIME:
            {
                slice = threads[next_tid].sched_params.real_time.slice;
                break;
            }
            default:
            {
                slice = DEFAULT_SCHED_SLICE;
                break;
            }
        }

        next_irq = MIN(next_irq, systime + sched_duration_to_systime(slice));

        threads[next_tid].status = THREAD_STATUS_RUNNING;
        threads[next_tid].start_time = mcu_systime_get_current();
        current_pid = threads[next_tid].pid;
        current_tid = next_tid;

        mcu_systime_set_comparator(next_irq);
        uspace_enter(&threads[next_tid].regs);
    }

    if (next_irq == UINT64_MAX)
    {
        mcu_systime_set_comparator(DEFAULT_SCHED_SLICE);
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

static void sched_save_thread(const regs_t *regs)
{
    const uint64_t systime = mcu_systime_get_current();
    const uint64_t delta = systime - threads[current_tid].start_time;

    threads[current_tid].exit_time = systime;
    threads[current_tid].regs = *regs;

    if (threads[current_tid].budget > 0)
    {
        if (threads[current_tid].budget >= delta)
        {
            threads[current_tid].budget -= delta;
        }
        else
        {
            threads[current_tid].budget = 0;
        }
    }
}

static uint64_t sched_duration_to_systime(const uint32_t duration)
{
    const uint64_t freq = mcu_sysclk_get_freq();
    const uint64_t delta = duration * freq / 1000;

    return delta;
}