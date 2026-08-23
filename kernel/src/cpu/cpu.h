#ifndef CPU_H
#define CPU_H

#include <stdint.h>

void uart_send(const char *str);

bool systime_init();
uint64_t systime_get_current();

#endif