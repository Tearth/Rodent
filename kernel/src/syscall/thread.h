#ifndef KERNEL_THREAD_H
#define KERNEL_THREAD_H

#include <shared/syscall.h>
#include "arch.h"
#include "sched.h"

void syscall_thread_get_pid(syscall_thread_get_pid_data_t *data);
void syscall_thread_get_tid(syscall_thread_get_tid_data_t *data);
void syscall_thread_get_sched(syscall_thread_get_sched_data_t *data);
void syscall_thread_set_sched(syscall_thread_set_sched_data_t *data);
void syscall_thread_sleep(regs_t *regs, syscall_thread_sleep_data_t *data);

#endif