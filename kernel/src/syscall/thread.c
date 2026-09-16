#include "thread.h"

void syscall_thread_sleep(regs_t *regs)
{
    syscall_thread_sleep_t *data = (syscall_thread_sleep_t *)regs->a1;
    sched_sleep(regs, data->duration);
}