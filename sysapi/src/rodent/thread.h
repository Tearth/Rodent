#ifndef SYSAPI_THREAD_H
#define SYSAPI_THREAD_H

#include <stdint.h>
#include <shared/syscall.h>
#include "arch.h"

uint8_t get_pid();
uint8_t get_priority(const uint8_t pid);
bool set_priority(const uint8_t priority);
void sleep(const uint32_t duration);

#endif