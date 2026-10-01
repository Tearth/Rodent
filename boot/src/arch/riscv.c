#include "arch.h"

[[noreturn]] void jmp(const void *addr, const void *args)
{
    __asm__ volatile (
        "fence.i\n"
        "mv a0, %0\n"
        "jalr x0, 0(%1)\n"
    : : "r"(args), "r"(addr) : "a0");

    __builtin_unreachable();
}