#ifndef SYSAPI_CPU_H
#define SYSAPI_CPU_H

#include <stdint.h>
#include <shared/syscall.h>
#include "arch.h"

uint64_t get_systime();
uint8_t get_cpu_cores();
uint32_t get_cpu_freq();

#endif