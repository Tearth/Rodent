#ifndef ARCH_H
#define ARCH_H

#ifdef ARCH_RISCV
#include "riscv/irq.h"
#include "riscv/uspace.h"
#endif

bool arch_init();

void arch_irq_enable();
void arch_attach_timer_handler(void (*handler)(regs_t *regs));
void arch_attach_ecall_handler(void (*handler)());

#endif