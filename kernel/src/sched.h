#ifndef KERNEL_SCHED_H
#define KERNEL_SCHED_H

#include <string.h>
#include <shared/boot.h>
#include <shared/def.h>
#include <shared/macro.h>
#include "arch.h"
#include "mcu.h"

typedef enum proc_status
{
    PROC_STATUS_NONE,
    PROC_STATUS_RUNNING
} proc_status_t;

typedef enum thread_status
{
    THREAD_STATUS_NONE,
    THREAD_STATUS_IDLE,
    THREAD_STATUS_SLEEPING,
    THREAD_STATUS_RUNNING
} thread_status_t;

typedef struct proc
{
    char path[MAX_PATH_LEN];
    void *base;
    void *entry;
    uint32_t size;
    proc_status_t status;
} proc_t;

typedef struct thread
{
    uint8_t pid;
    thread_status_t status;
    uint8_t priority;
    uint64_t deadline;
    regs_t regs;
} thread_t;

void sched_init(const boot_proc_t *boot_procs);
void sched_run();
void sched_timer_handler(regs_t *regs);
void sched_sleep(regs_t *regs, const uint32_t duration);

uint8_t sched_get_current_pid();
uint8_t sched_get_current_tid();
uint8_t sched_get_priority(const uint8_t tid);
bool sched_set_priority(const uint8_t tid, const uint8_t priority);

#endif