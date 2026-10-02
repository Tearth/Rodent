#include "cpu.h"

void syscall_cpu_get_systime([[maybe_unused]] const uint32_t cid, [[maybe_unused]] regs_t *regs, syscall_cpu_get_systime_data_t *data)
{
    data->systime = mcu_systime_get_current();
}