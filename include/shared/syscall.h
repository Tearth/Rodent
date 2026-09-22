#ifndef SHARED_SYSCALL_H
#define SHARED_SYSCALL_H

#include <stdint.h>
#include "syscall/thread.h"

typedef enum syscall
{
    SYSCALL_THREAD_GET_PID,
    SYSCALL_THREAD_GET_TID,
    SYSCALL_THREAD_GET_SCHED,
    SYSCALL_THREAD_SET_SCHED,
    SYSCALL_THREAD_SLEEP
} syscall_t;

#endif