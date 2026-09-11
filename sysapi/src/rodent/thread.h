#ifndef SYSAPI_THREAD_H
#define SYSAPI_THREAD_H

#include <stdint.h>
#include "arch/arch.h"
#include "shared/syscall.h"

void sleep(uint32_t duration);

#endif