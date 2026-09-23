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
    THREAD_STATUS_READY,
    THREAD_STATUS_WAITING,
    THREAD_STATUS_RUNNING
} thread_status_t;

typedef enum sched_policy
{
    SCHED_POLICY_ROUND_ROBIN,
    SCHED_POLICY_REAL_TIME,
    SCHED_POLICY_INVALID = -1,
} sched_policy_t;

typedef struct sched_rr_params
{
    uint32_t slice;
    uint8_t priority;
} sched_rr_params_t;

typedef struct sched_rt_params
{
    uint32_t budget;
    uint32_t deadline;
    uint32_t period;
    uint32_t slice;
    uint8_t priority_low;
    uint8_t priority_high;
} sched_rt_params_t;

typedef union sched_params
{
    sched_rr_params_t round_robin;
    sched_rt_params_t real_time;
} sched_params_t;

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
    regs_t regs;
    thread_status_t status;

    uint8_t priority;
    uint64_t awake_time;
    uint64_t start_time;
    uint64_t exit_time;
    uint32_t budget;
    uint64_t deadline;
    uint64_t replenishment;

    sched_policy_t sched_policy;
    sched_params_t sched_params;
} thread_t;

void sched_init(const boot_proc_t *boot_procs);
void sched_run();

uint8_t sched_get_current_pid();
uint8_t sched_get_current_tid();

sched_policy_t sched_get_policy(const uint8_t tid);
bool sched_set_policy(const uint8_t tid, const sched_policy_t policy);
bool sched_get_params(const uint8_t tid, sched_params_t *params);
bool sched_set_params(const uint8_t tid, const sched_params_t *params);

void sched_timer_handler(regs_t *regs);
void sched_sleep(regs_t *regs, const uint32_t duration);
void sched_yield_thread(regs_t *regs);
void sched_yield_budget(regs_t *regs);
void sched_yield_period(regs_t *regs);

#endif