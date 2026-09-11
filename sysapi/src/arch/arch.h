#ifndef SYSAPI_ARCH_H
#define SYSAPI_ARCH_H

#include "shared/syscall.h"

void syscall(syscall_t type, void *data);

#endif