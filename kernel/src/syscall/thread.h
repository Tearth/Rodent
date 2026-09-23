#ifndef KERNEL_THREAD_H
#define KERNEL_THREAD_H

#include <shared/macro.h>
#include <shared/syscall.h>
#include "arch.h"
#include "sched.h"

void syscall_thread_get_pid(regs_t *regs, syscall_thread_get_pid_data_t *data);
void syscall_thread_get_tid(regs_t *regs, syscall_thread_get_tid_data_t *data);
void syscall_thread_get_sched(regs_t *regs, syscall_thread_get_sched_data_t *data);
void syscall_thread_set_sched(regs_t *regs, syscall_thread_set_sched_data_t *data);
void syscall_thread_sleep(regs_t *regs, syscall_thread_sleep_data_t *data);
void syscall_thread_yield_thread(regs_t *regs, syscall_thread_yield_thread_data_t *data);
void syscall_thread_yield_budget(regs_t *regs, syscall_thread_yield_budget_data_t *data);
void syscall_thread_yield_period(regs_t *regs, syscall_thread_yield_period_data_t *data);

#endif