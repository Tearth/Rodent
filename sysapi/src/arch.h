#ifndef SYSAPI_ARCH_H
#define SYSAPI_ARCH_H

#include <shared/syscall.h>

void syscall(const syscall_t type, volatile void *data);

#endif