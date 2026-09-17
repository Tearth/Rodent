#include "thread.h"

void syscall_thread_get_pid(syscall_thread_get_pid_data_t *data)
{
    data->pid = sched_get_current_pid();
}

void syscall_thread_get_tid(syscall_thread_get_tid_data_t *data)
{
    data->tid = sched_get_current_tid();
}

void syscall_thread_get_priority(syscall_thread_get_priority_data_t *data)
{
    data->priority = sched_get_priority(data->tid);
}

void syscall_thread_set_priority(syscall_thread_set_priority_data_t *data)
{
    data->success = sched_set_priority(sched_get_current_tid(), data->priority);
}

void syscall_thread_sleep(regs_t *regs, syscall_thread_sleep_data_t *data)
{
    sched_sleep(regs, data->duration);
}