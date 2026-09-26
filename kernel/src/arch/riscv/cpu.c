#include "cpu.h"

uint32_t cpu_get_hart_id()
{
    uint32_t id;

    // Read MHARTID
    __asm__ volatile (
        "csrr %0, mhartid"
    : "=r"(id));

    return id;
}