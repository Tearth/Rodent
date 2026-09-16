#include "arch.h"

void syscall(syscall_t type, void *data)
{
    __asm__ volatile (
        "mv a0, %0\n"
        "mv a1, %1\n"
        "ecall\n"
    : : "r"(type), "r"(data) : "a0", "a1");
}