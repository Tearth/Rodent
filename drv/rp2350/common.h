#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define REG(addr) ((uint32_t*)addr)
#define WAIT(func, timeout) \
{ \
    int t = timeout; \
    while (func && t--); \
    if (t == 0) return false; \
}

#endif