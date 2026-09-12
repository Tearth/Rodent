#include "syscall.h"

void syscall_init()
{
    arch_attach_syscall_handler(syscall_irq_handler);
}

void syscall_irq_handler(regs_t *regs)
{
    switch ((syscall_t)regs->a0)
    {
        case SYSCALL_THREAD_SLEEP: syscall_thread_sleep(regs);
    }
}