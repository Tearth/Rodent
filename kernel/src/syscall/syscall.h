#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include "arch/arch.h"
#include "shared/syscall.h"
#include "thread.h"

void syscall_init();
void syscall_irq_handler(regs_t *regs);

#endif