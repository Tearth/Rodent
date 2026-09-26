#ifndef KERNEL_ARCH_H
#define KERNEL_ARCH_H

#ifdef ARCH_RISCV
#include "arch/riscv/irq.h"
#include "arch/riscv/sync.h"
#include "arch/riscv/uspace.h"
#endif

bool arch_init();

uint32_t arch_cpu_get_cid();

void arch_irq_enable();
void arch_irq_wait();
void arch_attach_timer_handler(void (*handler)(regs_t *regs));
void arch_attach_syscall_handler(void (*handler)(regs_t *regs));

void arch_mutex_lock(mutex_t *mutex);
void arch_mutex_unlock(mutex_t *mutex);

#endif