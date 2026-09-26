#include "ext.h"

#if defined(ARCH_RISCV)
void ext_hazard3_unblock()
{
    __asm__ volatile ("slt x0, x0, x1" ::: "x0", "x1");
}
#endif