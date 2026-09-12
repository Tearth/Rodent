#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <shared/syscall.h>
#include <syscall/thread.h>
#include "arch.h"

void syscall_init();
void syscall_irq_handler(regs_t *regs);

#endif