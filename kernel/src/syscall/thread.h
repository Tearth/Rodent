#ifndef KERNEL_THREAD_H
#define KERNEL_THREAD_H

#include <shared/syscall.h>
#include "arch.h"
#include "sched.h"

void syscall_thread_sleep(regs_t *regs, syscall_thread_sleep_t *data);

#endif