#ifndef KERNEL_ARCH_RISCV_USPACE_H
#define KERNEL_ARCH_RISCV_USPACE_H

#include "reg.h"

[[noreturn]] void uspace_enter(regs_t *regs);

#endif