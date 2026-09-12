#ifndef KERNEL_THREAD_H
#define KERNEL_THREAD_H

#include <shared/syscall.h>
#include "arch.h"

void syscall_thread_sleep(regs_t *regs);

#endif