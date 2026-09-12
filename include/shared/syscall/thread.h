#ifndef SHARED_THREAD_H
#define SHARED_THREAD_H

#include <stdint.h>

typedef struct syscall_thread_sleep
{
    uint32_t duration;
} syscall_thread_sleep_t;

#endif