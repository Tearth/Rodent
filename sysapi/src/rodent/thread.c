#include "thread.h"

uint8_t get_pid()
{
    syscall_thread_get_pid_data_t data =
    {

    };
    syscall(SYSCALL_THREAD_GET_PID, &data);

    return data.pid;
}

uint8_t get_tid()
{
    syscall_thread_get_tid_data_t data =
    {

    };
    syscall(SYSCALL_THREAD_GET_TID, &data);

    return data.tid;
}

uint8_t get_priority(const uint8_t tid)
{
    syscall_thread_get_priority_data_t data =
    {
        .tid = tid
    };
    syscall(SYSCALL_THREAD_GET_PRIORITY, &data);

    return data.priority;
}

bool set_priority(const uint8_t priority)
{
    syscall_thread_set_priority_data_t data =
    {
        .priority = priority
    };
    syscall(SYSCALL_THREAD_SET_PRIORITY, &data);

    return data.success;
}

void sleep(const uint32_t duration)
{
    syscall_thread_sleep_data_t data =
    {
        .duration = duration
    };
    syscall(SYSCALL_THREAD_SLEEP, &data);
}