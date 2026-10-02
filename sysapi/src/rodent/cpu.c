#include "cpu.h"

uint64_t get_systime()
{
    syscall_cpu_get_systime_data_t data =
    {

    };
    syscall(SYSCALL_CPU_GET_SYSTIME, &data);

    return data.systime;
}

uint32_t get_cpu_freq()
{
    syscall_cpu_get_freq_data_t data =
    {

    };
    syscall(SYSCALL_CPU_GET_FREQ, &data);

    return data.freq;
}