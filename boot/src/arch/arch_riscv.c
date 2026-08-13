#include "arch.h"

__attribute__((noreturn)) void jmp(void *addr)
{
    __asm__ (
        "fence.i\n" \
        "jalr  x0, 0(%0)"
    : : "r"(addr));

    __builtin_unreachable();
}