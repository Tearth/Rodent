#ifndef INCLUDE_SYSCALL_H
#define INCLUDE_SYSCALL_H

#include <stdint.h>
#include "syscall/thread.h"

typedef enum syscall
{
    SYSCALL_THREAD_SLEEP
} syscall_t;

#endif