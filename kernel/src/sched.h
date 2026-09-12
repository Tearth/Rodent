#ifndef KERNEL_SCHED_H
#define KERNEL_SCHED_H

#include <string.h>
#include <shared/def.h>
#include <shared/boot.h>
#include <shared/math.h>
#include "arch.h"
#include "mcu.h"

typedef enum proc_status
{
    PROC_STATUS_NONE,
    PROC_STATUS_IDLE,
    PROC_STATUS_SLEEPING,
    PROC_STATUS_RUNNING
} proc_status_t;

typedef struct proc
{
    char path[MAX_PATH_LEN];

    void *base;
    void *entry;
    uint32_t size;

    proc_status_t status;
    uint64_t deadline;
    regs_t regs;
} proc_t;

void sched_init(boot_proc_t *boot_procs);
void sched_run();
void sched_irq_handler(regs_t *regs);
void sched_sleep(regs_t *regs, uint32_t duration);

#endif