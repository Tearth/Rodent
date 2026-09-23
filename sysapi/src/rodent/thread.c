#include "thread.h"

uint8_t get_pid()
{
    syscall_thread_get_pid_data_t data =
    {

    };
    syscall(SYSCALL_THREAD_GET_PID, &data);

    return data.pid;
}

uint8_t get_tid()
{
    syscall_thread_get_tid_data_t data =
    {

    };
    syscall(SYSCALL_THREAD_GET_TID, &data);

    return data.tid;
}

void get_sched(sched_policy_t *policy, sched_params_t *params)
{
    syscall_thread_get_sched_data_t data =
    {

    };
    syscall(SYSCALL_THREAD_GET_SCHED, &data);

    *policy = (sched_policy_t)data.policy;
    *params = data.params;
}

bool set_sched(const sched_policy_t policy, const sched_params_t *params)
{
    syscall_thread_set_sched_data_t data =
    {
        .policy = (syscall_thread_sched_policy_t)policy,
        .params = *params
    };
    syscall(SYSCALL_THREAD_SET_SCHED, &data);

    return data.success;
}

void sleep(const uint32_t duration)
{
    syscall_thread_sleep_data_t data =
    {
        .duration = duration
    };
    syscall(SYSCALL_THREAD_SLEEP, &data);
}

void yield_thread()
{
    syscall_thread_yield_thread_data_t data =
    {

    };
    syscall(SYSCALL_THREAD_YIELD_THREAD, &data);
}

void yield_budget()
{
    syscall_thread_yield_budget_data_t data =
    {

    };
    syscall(SYSCALL_THREAD_YIELD_BUDGET, &data);
}

void yield_period()
{
    syscall_thread_yield_period_data_t data =
    {

    };
    syscall(SYSCALL_THREAD_YIELD_PERIOD, &data);
}