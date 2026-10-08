#ifndef KERNEL_ARCH_RISCV_SYNC_H
#define KERNEL_ARCH_RISCV_SYNC_H

#include <stddef.h>
#include <stdint.h>

typedef struct mutex
{
    bool flag;
} mutex_t;

bool mutex_lock(mutex_t *mutex);
bool mutex_unlock(mutex_t *mutex);

#endif