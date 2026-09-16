#ifndef BOOT_ARCH_H
#define BOOT_ARCH_H

__attribute__((noreturn)) void jmp(const void *addr, const void *args);

#endif