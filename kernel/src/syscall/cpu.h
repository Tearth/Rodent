#ifndef KERNEL_CPU_H
#define KERNEL_CPU_H

#include <shared/macro.h>
#include <shared/syscall.h>
#include "arch.h"
#include "sched.h"

void syscall_cpu_get_systime(const uint32_t cid, regs_t *regs, syscall_cpu_get_systime_data_t *data);

#endif