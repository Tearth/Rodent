#ifndef SYSAPI_THREAD_H
#define SYSAPI_THREAD_H

#include <stdint.h>
#include <shared/syscall.h>
#include "arch.h"

typedef enum sched_policy
{
    SCHED_POLICY_ROUND_ROBIN = SYSCALL_THREAD_SCHED_POLICY_ROUND_ROBIN,
    SCHED_POLICY_REAL_TIME = SYSCALL_THREAD_SCHED_POLICY_REAL_TIME,
} sched_policy_t;
typedef syscall_thread_sched_params_t sched_params_t;

uint8_t get_pid();
uint8_t get_tid();
void get_sched(const uint8_t tid, sched_policy_t *policy, sched_params_t *params);
bool set_sched(const sched_policy_t policy, const sched_params_t *params);
void get_cpu_ticks(const uint8_t tid, uint64_t *ticks);
void sleep(const uint32_t duration);
void yield_thread();
void yield_budget();
void yield_period();

#endif