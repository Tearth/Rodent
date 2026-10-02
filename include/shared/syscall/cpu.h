#ifndef SHARED_CPU_H
#define SHARED_CPU_H

typedef struct syscall_cpu_get_systime_data
{
    // Response
    uint64_t systime;
} syscall_cpu_get_systime_data_t;

typedef struct syscall_cpu_get_freq_data
{
    // Response
    uint64_t freq;
} syscall_cpu_get_freq_data_t;

#endif