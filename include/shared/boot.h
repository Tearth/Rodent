#ifndef SHARED_BOOT_H
#define SHARED_BOOT_H

#include <stdarg.h>
#include <stdint.h>

#define MAX_BOOT_PROCS 8

typedef enum boot_proc_type
{
    BOOT_PROC_TYPE_NONE,
    BOOT_PROC_TYPE_SRV
} boot_proc_type_t;

typedef struct boot_proc
{
    char path[UINT8_MAX];
    boot_proc_type_t type;

    void *base;
    void *entry;
    uint32_t size;
} boot_proc_t;

typedef struct boot_args
{
    boot_proc_t procs[MAX_BOOT_PROCS];
} boot_args_t;

#endif