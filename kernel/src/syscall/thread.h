#ifndef KERNEL_SYSCALL_THREAD_H
#define KERNEL_SYSCALL_THREAD_H

#include <shared/macro.h>
#include <shared/syscall.h>
#include "arch.h"
#include "sched.h"

void syscall_thread_get_pid(const uint32_t cid, regs_t *regs, syscall_thread_get_pid_data_t *data);
void syscall_thread_get_tid(const uint32_t cid, regs_t *regs, syscall_thread_get_tid_data_t *data);
void syscall_thread_get_sched(const uint32_t cid, regs_t *regs, syscall_thread_get_sched_data_t *data);
void syscall_thread_set_sched(const uint32_t cid, regs_t *regs, syscall_thread_set_sched_data_t *data);
void syscall_thread_get_cpu_ticks(const uint32_t cid, regs_t *regs, syscall_thread_get_cpu_ticks_data_t *data);
void syscall_thread_sleep(const uint32_t cid, regs_t *regs, syscall_thread_sleep_data_t *data);
void syscall_thread_yield_thread(const uint32_t cid, regs_t *regs, syscall_thread_yield_thread_data_t *data);
void syscall_thread_yield_budget(const uint32_t cid, regs_t *regs, syscall_thread_yield_budget_data_t *data);
void syscall_thread_yield_period(const uint32_t cid, regs_t *regs, syscall_thread_yield_period_data_t *data);

#endif