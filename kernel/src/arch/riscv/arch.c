#include <stdint.h>
#include "arch/arch.h"
#include "irq.h"
#include "pmp.h"
#include "log.h"

extern uint32_t __kernel_start;
extern uint32_t __kernel_end;

static bool arch_init_irq();
static bool arch_init_pmp();

bool arch_init()
{
    if (!arch_init_irq())
    {
        return false;
    }

    if (!arch_init_pmp())
    {
        return false;
    }

    return true;
}

bool arch_init_irq()
{
    if (!irq_enable())
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to init interrupts"), false;
    }

    log_msg(LOG_LEVEL_OK, "Initialized interrupts");

    return true;
}

bool arch_init_pmp()
{
    uint32_t kernel_start = (uint32_t)&__kernel_start;
    uint32_t kernel_end = (uint32_t)&__kernel_end;
    uint32_t size = kernel_end - kernel_start;

    pmp_set_area(PMP_REGION0, (void*)kernel_start, size);
    pmp_set_rwx(PMP_REGION0, false, false, false);
    pmp_set_mode(PMP_REGION0, PMP_MODE_NATOP);

    char region_from_buf[16];
    char region_to_buf[16];
    char region_size_buf[16];

    itoa(kernel_start, region_from_buf, 16);
    itoa(kernel_end, region_to_buf, 16);
    itoa(size / 1024, region_size_buf, 10);

    log_msg(LOG_LEVEL_OK, "Initialized PMP for kernel region");
    log_fmt(LOG_LEVEL_INFO, " PMP_REGION0 @ 0x", region_from_buf, "-0x", region_to_buf, " (", region_size_buf, " KB)", EOL);

    return true;
}