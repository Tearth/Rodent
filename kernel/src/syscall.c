#include "syscall.h"

void syscall_init()
{
    arch_attach_syscall_handler(syscall_handler);
}

void syscall_handler(const uint32_t cid, regs_t *regs)
{
    switch ((syscall_t)regs->a0)
    {
        case SYSCALL_CPU_GET_SYSTIME: syscall_cpu_get_systime(cid, regs, (syscall_cpu_get_systime_data_t *)regs->a1); break;
        case SYSCALL_THREAD_GET_PID: syscall_thread_get_pid(cid, regs, (syscall_thread_get_pid_data_t *)regs->a1); break;
        case SYSCALL_THREAD_GET_TID: syscall_thread_get_tid(cid, regs, (syscall_thread_get_tid_data_t *)regs->a1); break;
        case SYSCALL_THREAD_GET_SCHED: syscall_thread_get_sched(cid, regs, (syscall_thread_get_sched_data_t *)regs->a1); break;
        case SYSCALL_THREAD_SET_SCHED: syscall_thread_set_sched(cid, regs, (syscall_thread_set_sched_data_t *)regs->a1); break;
        case SYSCALL_THREAD_SLEEP: syscall_thread_sleep(cid, regs, (syscall_thread_sleep_data_t *)regs->a1); break;
        case SYSCALL_THREAD_YIELD_THREAD: syscall_thread_yield_thread(cid, regs, (syscall_thread_yield_thread_data_t *)regs->a1); break;
        case SYSCALL_THREAD_YIELD_BUDGET: syscall_thread_yield_budget(cid, regs, (syscall_thread_yield_budget_data_t *)regs->a1); break;
        case SYSCALL_THREAD_YIELD_PERIOD: syscall_thread_yield_period(cid, regs, (syscall_thread_yield_period_data_t *)regs->a1); break;
    }
}