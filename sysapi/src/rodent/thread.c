#include "thread.h"

void sleep(uint32_t duration)
{
    syscall_thread_sleep_t data =
    {
        duration = duration
    };
    syscall(SYSCALL_THREAD_SLEEP, &data);
}