#ifndef SYSAPI_THREAD_H
#define SYSAPI_THREAD_H

#include <stdint.h>
#include <shared/syscall.h>
#include "arch.h"

void sleep(uint32_t duration);

#endif