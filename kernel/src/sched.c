#include "sched.h"

extern uint32_t __stack_pointer;

static bool enabled = false;
static uint32_t systime_freq = 0;

static proc_t procs[MAX_PROCS] = {};
static thread_t threads[MAX_THREADS] = {};
static uint8_t current_pid[CPU_CORES];
static uint8_t current_tid[CPU_CORES];

[[noreturn]] static void sched_next(const uint32_t cid);
static void sched_save_thread(const uint32_t cid, const regs_t *regs);
static uint64_t sched_duration_to_systime(const uint32_t duration);

void sched_init(const boot_proc_t *boot_procs)
{
    systime_freq = mcu_sysclk_get_freq();

    for (size_t cid = 0; cid < CPU_CORES; cid++)
    {
        current_pid[cid] = UINT8_MAX;
        current_tid[cid] = UINT8_MAX;
    }

    for (size_t i = 0; i < MAX_BOOT_PROCS; i++)
    {
        if (boot_procs[i].type == BOOT_PROC_TYPE_NONE)
        {
            continue;
        }

        memcpy(procs[i].path, boot_procs[i].path, UINT8_MAX);

        procs[i].status = PROC_STATUS_RUNNING;
        procs[i].base = boot_procs[i].base;
        procs[i].entry = boot_procs[i].entry;
        procs[i].size = boot_procs[i].size;

        threads[i].pid = i;
        threads[i].cid = i % CPU_CORES;
        threads[i].regs.pc = (uint32_t)boot_procs[i].entry;
        threads[i].status = THREAD_STATUS_READY;
        threads[i].priority = 0;
        threads[i].awake_time = 0;
        threads[i].start_time = 0;
        threads[i].exit_time = 0;
        threads[i].budget = 0;
        threads[i].deadline = 0;
        threads[i].replenishment = 0;

        threads[i].sched_policy = SCHED_POLICY_ROUND_ROBIN;
        threads[i].sched_params.round_robin.slice = 1;
        threads[i].sched_params.round_robin.priority = 0;

        for (size_t cid = 0; cid < CPU_CORES; cid++)
        {
            threads[i].cpu_ticks[cid] = 0;
        }
    }

    arch_attach_timer_handler(sched_timer_handler);
    log_msg(LOG_LEVEL_OK, "Initialized scheduler");
}

void sched_enable()
{
    enabled = true;
}

void sched_disable()
{
    enabled = false;
}

bool sched_is_enabled()
{
    return enabled;
}

[[noreturn]] void sched_run()
{
    sched_next(arch_cpu_get_cid());
}

uint8_t sched_get_current_pid(const uint32_t cid)
{
    return current_pid[cid];
}

uint8_t sched_get_current_tid(const uint32_t cid)
{
    return current_tid[cid];
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

    return threads[tid].sched_policy = policy, true;
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
            threads[tid].budget = sched_duration_to_systime(params->real_time.budget);
            threads[tid].deadline = systime + sched_duration_to_systime(params->real_time.deadline);
            threads[tid].replenishment = systime + sched_duration_to_systime(params->real_time.period);
            break;
        }
        default: return false;
    }

    return threads[tid].sched_params = *params, true;
}

void sched_get_cpu_ticks(const uint8_t tid, uint64_t *ticks)
{
    memcpy(ticks, threads[tid].cpu_ticks, sizeof(threads[tid].cpu_ticks));
}

void sched_timer_handler(const uint32_t cid, regs_t *regs)
{
    if (current_tid[cid] != UINT8_MAX)
    {
        threads[current_tid[cid]].status = THREAD_STATUS_READY;
        sched_save_thread(cid, regs);
    }

    sched_next(cid);
}

void sched_sleep(const uint32_t cid, regs_t *regs, const uint32_t duration)
{
    const uint64_t systime = mcu_systime_get_current();

    threads[current_tid[cid]].status = THREAD_STATUS_WAITING;
    threads[current_tid[cid]].awake_time = systime + sched_duration_to_systime(duration);

    sched_save_thread(cid, regs);
    sched_next(cid);
}

void sched_yield_thread(const uint32_t cid, regs_t *regs)
{
    threads[current_tid[cid]].status = THREAD_STATUS_READY;

    sched_save_thread(cid, regs);
    sched_next(cid);
}

void sched_yield_budget(const uint32_t cid, regs_t *regs)
{
    if (threads[current_tid[cid]].sched_policy == SCHED_POLICY_REAL_TIME)
    {
        threads[current_tid[cid]].status = THREAD_STATUS_READY;
        threads[current_tid[cid]].budget = 0;
    }

    sched_save_thread(cid, regs);
    sched_next(cid);
}

void sched_yield_period(const uint32_t cid, regs_t *regs)
{
    if (threads[current_tid[cid]].sched_policy == SCHED_POLICY_REAL_TIME)
    {
        threads[current_tid[cid]].status = THREAD_STATUS_WAITING;
        threads[current_tid[cid]].awake_time = threads[current_tid[cid]].replenishment;
    }

    sched_save_thread(cid, regs);
    sched_next(cid);
}

static void sched_next(const uint32_t cid)
{
    const uint64_t systime = mcu_systime_get_current();

    uint8_t next_tid = UINT8_MAX;
    uint64_t next_irq = UINT64_MAX;

    if (!enabled)
    {
        goto idle;
    }

    if (threads[current_tid[cid]].start_time > 0)
    {
        threads[current_tid[cid]].cpu_ticks[cid] += systime - threads[current_tid[cid]].start_time;
    }

    // Wake up threads from sleeping
    for (size_t tid = 0; tid < MAX_THREADS; tid++)
    {
        if (threads[tid].cid != cid)
        {
            continue;
        }

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
                const uint64_t budget_systime = sched_duration_to_systime(threads[tid].sched_params.real_time.budget);
                const uint64_t deadline_systime = sched_duration_to_systime(threads[tid].sched_params.real_time.deadline);
                const uint64_t period_systime = sched_duration_to_systime(threads[tid].sched_params.real_time.period);

                threads[tid].priority = threads[tid].sched_params.real_time.priority_high;
                threads[tid].budget = budget_systime;
                threads[tid].deadline += period_systime;
                threads[tid].replenishment += period_systime;

                // If thread was stalled and replenishment is not up with sysclock, start with fresh base
                if (threads[tid].replenishment <= systime)
                {
                    threads[tid].deadline = systime + deadline_systime;
                    threads[tid].replenishment = systime + period_systime;
                }

                next_irq = MIN(next_irq, threads[tid].replenishment);
            }

            // Decrease priority when budget has been used
            if (threads[tid].budget == 0)
            {
                threads[tid].priority = threads[tid].sched_params.real_time.priority_low;
            }
        }
    }

    for (size_t p = 0; p < UINT8_MAX && next_tid == UINT8_MAX; p++)
    {
        // Find any thread with real time policy that is ready to run
        for (size_t tid = 0; tid < MAX_THREADS; tid++)
        {
            if (threads[tid].cid != cid || threads[tid].priority != p)
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
                if (threads[tid].cid != cid || threads[tid].priority != p)
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
                slice = 1;
                break;
            }
        }

        next_irq = MIN(next_irq, systime + sched_duration_to_systime(slice));

        threads[next_tid].status = THREAD_STATUS_RUNNING;
        threads[next_tid].start_time = mcu_systime_get_current();
        current_pid[cid] = threads[next_tid].pid;
        current_tid[cid] = next_tid;

        mcu_systime_set_comparator(next_irq);
        uspace_enter(&threads[next_tid].regs);
    }

idle:
    if (next_irq == UINT64_MAX)
    {
        mcu_systime_set_comparator(systime + sched_duration_to_systime(1));
    }

    // No available thread was found, wait for the next interrupt
    current_pid[cid] = UINT8_MAX;
    current_tid[cid] = UINT8_MAX;

    while (1)
    {
        arch_irq_enable();
        arch_irq_wait();
    }
}

static void sched_save_thread(const uint32_t cid, const regs_t *regs)
{
    const uint64_t systime = mcu_systime_get_current();
    const uint64_t delta = systime - threads[current_tid[cid]].start_time;

    threads[current_tid[cid]].exit_time = systime;
    threads[current_tid[cid]].regs = *regs;

    if (threads[current_tid[cid]].budget > 0)
    {
        if (threads[current_tid[cid]].budget >= delta)
        {
            threads[current_tid[cid]].budget -= delta;
        }
        else
        {
            threads[current_tid[cid]].budget = 0;
        }
    }
}

static uint64_t sched_duration_to_systime(const uint32_t duration)
{
    return (uint64_t)duration * systime_freq / 1000;
}