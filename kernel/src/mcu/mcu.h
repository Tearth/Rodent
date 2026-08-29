#ifndef MCU_H
#define MCU_H

#include <stdint.h>

bool systime_init();
uint64_t systime_get_current();

#endif