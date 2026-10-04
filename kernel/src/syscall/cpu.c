#include "cpu.h"

void syscall_cpu_get_systime([[maybe_unused]] const uint32_t cid, [[maybe_unused]] regs_t *regs, syscall_cpu_get_systime_data_t *data)
{
    data->systime = mcu_systime_get_current();
}

void syscall_cpu_get_cores([[maybe_unused]] const uint32_t cid, [[maybe_unused]] regs_t *regs, syscall_cpu_get_cores_data_t *data)
{
    data->cores = CPU_CORES;
}

void syscall_cpu_get_freq([[maybe_unused]] const uint32_t cid, [[maybe_unused]] regs_t *regs, syscall_cpu_get_freq_data_t *data)
{
    data->freq = mcu_sysclk_get_freq();
}