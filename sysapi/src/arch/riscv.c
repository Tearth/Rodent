#include <stdint.h>
#include "arch.h"

void syscall(const syscall_t type, volatile void *data)
{
    register uint32_t a0 __asm__ ("a0") = (uint32_t)type;
    register uint32_t a1 __asm__ ("a1") = (uint32_t)data;

    __asm__ volatile ("ecall" : : "r"(a0), "r"(a1));
}