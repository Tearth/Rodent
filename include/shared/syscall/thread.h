#ifndef SHARED_SYSCALL_THREAD_H
#define SHARED_SYSCALL_THREAD_H

#include <stdint.h>

typedef enum syscall_thread_sched_policy
{
    SYSCALL_THREAD_SCHED_POLICY_ROUND_ROBIN,
    SYSCALL_THREAD_SCHED_POLICY_REAL_TIME,
    SYSCALL_THREAD_SCHED_POLICY_INVALID = -1
} syscall_thread_sched_policy_t;

typedef struct syscall_thread_sched_rr_params
{
    uint32_t slice;
    uint8_t priority;
} syscall_thread_sched_rr_params_t;

typedef struct syscall_thread_sched_rt_params
{
    uint32_t budget;
    uint32_t deadline;
    uint32_t period;
    uint32_t slice;
    uint8_t priority_low;
    uint8_t priority_high;
} syscall_thread_sched_rt_params_t;

typedef union syscall_thread_sched_params
{
    syscall_thread_sched_rr_params_t round_robin;
    syscall_thread_sched_rt_params_t real_time;
} syscall_thread_sched_params_t;

typedef struct syscall_thread_get_pid_data
{
    // Response
    uint8_t pid;
} syscall_thread_get_pid_data_t;

typedef struct syscall_thread_get_tid_data
{
    // Response
    uint8_t tid;
} syscall_thread_get_tid_data_t;

typedef struct syscall_thread_get_priority_data
{
    // Request
    const uint8_t tid;

    // Response
    uint8_t priority;
} syscall_thread_get_priority_data_t;

typedef struct syscall_thread_set_priority_data
{
    // Request
    const uint8_t priority;

    // Response
    bool success;
} syscall_thread_set_priority_data_t;

typedef struct syscall_thread_get_sched_data
{
    // Request
    const uint8_t tid;

    // Response
    syscall_thread_sched_policy_t policy;
    syscall_thread_sched_params_t *params;
} syscall_thread_get_sched_data_t;

typedef struct syscall_thread_set_sched_data
{
    // Request
    const syscall_thread_sched_policy_t policy;
    const syscall_thread_sched_params_t *params;

    // Response
    bool success;
} syscall_thread_set_sched_data_t;

typedef struct syscall_thread_get_cpu_ticks_data
{
    // Request
    const uint8_t tid;

    // Response
    uint64_t *ticks;
} syscall_thread_get_cpu_ticks_data_t;

typedef struct syscall_thread_sleep_data
{
    // Request
    uint32_t duration;
} syscall_thread_sleep_data_t;

typedef struct syscall_thread_yield_thread_data
{

} syscall_thread_yield_thread_data_t;

typedef struct syscall_thread_yield_budget_data
{

} syscall_thread_yield_budget_data_t;

typedef struct syscall_thread_yield_period_data
{

} syscall_thread_yield_period_data_t;

#endif