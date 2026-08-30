#include "arch.h"
#include "common/boot.h"

__attribute__((noreturn)) void jmp(void *addr, void *iface, void *args)
{
    __asm__ volatile (
        "fence.i\n" \
        "mv a0, %0\n" \
        "mv a1, %1\n" \
        "jalr x0, 0(%2)\n" \
    : : "r"(iface), "r"(args), "r"(addr));

    __builtin_unreachable();
}