#ifndef SCHED_H
#define SCHED_H

#include <string.h>
#include "arch/arch.h"
#include "mcu/mcu.h"
#include "shared/defs.h"
#include "shared/boot.h"

typedef enum proc_status
{
    PROC_STATUS_NONE,
    PROC_STATUS_IDLE,
    PROC_STATUS_RUNNING
} proc_status_t;

typedef struct proc
{
    char path[MAX_PATH_LEN];

    void *base;
    void *entry;
    uint32_t size;

    proc_status_t status;
    regs_t regs;
} proc_t;

void sched_init(boot_proc_t *boot_procs);
void sched_run();

void sched_timer_handler(regs_t *regs);

#endif