#ifndef SHARED_BOOT_H
#define SHARED_BOOT_H

#include <stdarg.h>
#include <stdint.h>
#include "defs.h"
#include "log.h"

typedef struct boot_iface
{
    void (*log_msg)(log_level_t level, const char *msg);
    void (*log_vargs)(log_level_t level, const char *msg, va_list args);
} boot_iface_t;

typedef enum boot_proc_type
{
    BOOT_PROC_TYPE_NONE,
    BOOT_PROC_TYPE_SRV
} boot_proc_type_t;

typedef struct boot_proc
{
    char path[MAX_PATH_LEN];
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