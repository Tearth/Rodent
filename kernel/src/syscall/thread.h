#ifndef KERNEL_THREAD_H
#define KERNEL_THREAD_H

#include "arch/arch.h"
#include "shared/syscall.h"

void syscall_thread_sleep(regs_t *regs);

#endif