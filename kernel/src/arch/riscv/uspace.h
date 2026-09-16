#ifndef KERNEL_USPACE_H
#define KERNEL_USPACE_H

#include "reg.h"

__attribute__((noreturn)) void uspace_enter(regs_t *regs);

#endif