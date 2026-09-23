#include "syscall.h"

void syscall_init()
{
    arch_attach_syscall_handler(syscall_handler);
}

void syscall_handler(regs_t *regs)
{
    switch ((syscall_t)regs->a0)
    {
        case SYSCALL_THREAD_GET_PID: syscall_thread_get_pid(regs, (syscall_thread_get_pid_data_t *)regs->a1); break;
        case SYSCALL_THREAD_GET_TID: syscall_thread_get_tid(regs, (syscall_thread_get_tid_data_t *)regs->a1); break;
        case SYSCALL_THREAD_GET_SCHED: syscall_thread_get_sched(regs, (syscall_thread_get_sched_data_t *)regs->a1); break;
        case SYSCALL_THREAD_SET_SCHED: syscall_thread_set_sched(regs, (syscall_thread_set_sched_data_t *)regs->a1); break;
        case SYSCALL_THREAD_SLEEP: syscall_thread_sleep(regs, (syscall_thread_sleep_data_t *)regs->a1); break;
        case SYSCALL_THREAD_YIELD_THREAD: syscall_thread_yield_thread(regs, (syscall_thread_yield_thread_data_t *)regs->a1); break;
        case SYSCALL_THREAD_YIELD_BUDGET: syscall_thread_yield_budget(regs, (syscall_thread_yield_budget_data_t *)regs->a1); break;
        case SYSCALL_THREAD_YIELD_PERIOD: syscall_thread_yield_period(regs, (syscall_thread_yield_period_data_t *)regs->a1); break;
    }
}