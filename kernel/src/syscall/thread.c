#include "thread.h"

void syscall_thread_get_pid(syscall_thread_get_pid_data_t *data)
{
    data->pid = sched_get_current_pid();
}

void syscall_thread_get_tid(syscall_thread_get_tid_data_t *data)
{
    data->tid = sched_get_current_tid();
}

void syscall_thread_get_sched(syscall_thread_get_sched_data_t *data)
{
    const uint8_t tid = sched_get_current_tid();
    sched_params_t params;

    if (!sched_get_params(tid, &params))
    {
        data->policy = SYSCALL_THREAD_SCHED_POLICY_INVALID;
        return;
    }

    switch (sched_get_policy(tid))
    {
        case SCHED_POLICY_ROUND_ROBIN:
        {
            data->policy = SYSCALL_THREAD_SCHED_POLICY_ROUND_ROBIN;
            data->params.round_robin.slice = params.round_robin.slice;
            data->params.round_robin.priority = params.round_robin.priority;
            break;
        }
        case SCHED_POLICY_REAL_TIME:
        {
            data->policy = SYSCALL_THREAD_SCHED_POLICY_REAL_TIME;
            data->params.real_time.budget = params.real_time.budget;
            data->params.real_time.deadline = params.real_time.deadline;
            data->params.real_time.period = params.real_time.period;
            data->params.real_time.slice = params.real_time.slice;
            data->params.real_time.priority_low = params.real_time.priority_low;
            data->params.real_time.priority_high = params.real_time.priority_high;
            break;
        }
        case SCHED_POLICY_INVALID:
        {
            data->policy = SYSCALL_THREAD_SCHED_POLICY_INVALID;
            break;
        }
    }
}

void syscall_thread_set_sched(syscall_thread_set_sched_data_t *data)
{
    const uint8_t tid = sched_get_current_tid();
    const sched_policy_t policy_old = sched_get_policy(tid);
    sched_policy_t policy;
    sched_params_t params;

    switch (data->policy)
    {
        case SYSCALL_THREAD_SCHED_POLICY_ROUND_ROBIN:
        {
            policy = SCHED_POLICY_ROUND_ROBIN;
            params.round_robin.slice = data->params.round_robin.slice;
            params.round_robin.priority = data->params.round_robin.priority;
            break;
        }
        case SYSCALL_THREAD_SCHED_POLICY_REAL_TIME:
        {
            policy = SCHED_POLICY_REAL_TIME;
            params.real_time.budget = data->params.real_time.budget;
            params.real_time.deadline = data->params.real_time.deadline;
            params.real_time.period = data->params.real_time.period;
            params.real_time.slice = data->params.real_time.slice;
            params.real_time.priority_low = data->params.real_time.priority_low;
            params.real_time.priority_high = data->params.real_time.priority_high;
            break;
        }
        case SYSCALL_THREAD_SCHED_POLICY_INVALID:
        {
            data->success = false;
            return;
        }
    }

    if (!sched_set_policy(tid, policy))
    {
        data->success = false;
        return;
    }

    if (!sched_set_params(tid, &params))
    {
        sched_set_policy(tid, policy_old);
        data->success = false;
        return;
    }

    data->success = true;
}

void syscall_thread_sleep(regs_t *regs, syscall_thread_sleep_data_t *data)
{
    sched_sleep(regs, data->duration);
}