#include "arch.h"
#include "common/boot.h"

__attribute__((noreturn)) void jmp(void *addr, void *arg)
{
    __asm__ volatile (
        "fence.i\n" \
        "mv a0, %0\n" \
        "jalr x0, 0(%1)\n" \
    : : "r"(arg), "r"(addr));

    __builtin_unreachable();
}