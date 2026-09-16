#include <stdint.h>
#include "arch.h"
#include "irq.h"
#include "log.h"
#include "pmp.h"
#include "uspace.h"

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

static bool arch_init_irq()
{
    if (!irq_init())
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to initialize interrupts"), false;
    }

    return log_msg(LOG_LEVEL_OK, "Initialized interrupts"), true;
}

static bool arch_init_pmp()
{
    const uint32_t kernel_start = (uint32_t)&__kernel_start;
    const uint32_t kernel_end = (uint32_t)&__kernel_end;
    const uint32_t size = kernel_end - kernel_start;

    pmp_set_area(PMP_REGION0, (void *)kernel_start, size);
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

void arch_irq_enable()
{
    irq_enable();
}

void arch_irq_wait()
{
    irq_wait();
}

void arch_attach_timer_handler(void (*handler)(regs_t *regs))
{
    irq_attach_timer_handler(handler);
}

void arch_attach_syscall_handler(void (*handler)(regs_t *regs))
{
    irq_attach_syscall_handler(handler);
}