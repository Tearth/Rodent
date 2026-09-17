#ifndef SHARED_THREAD_H
#define SHARED_THREAD_H

#include <stdint.h>

typedef struct syscall_thread_get_pid_data
{
    // Response
    uint8_t pid;
} syscall_thread_get_pid_data_t;

typedef struct syscall_thread_get_priority_data
{
    // Request
    uint8_t pid;

    // Response
    uint8_t priority;
} syscall_thread_get_priority_data_t;

typedef struct syscall_thread_set_priority_data
{
    // Request
    uint8_t priority;

    // Response
    bool success;
} syscall_thread_set_priority_data_t;

typedef struct syscall_thread_sleep_data
{
    // Request
    uint32_t duration;
} syscall_thread_sleep_data_t;

#endif