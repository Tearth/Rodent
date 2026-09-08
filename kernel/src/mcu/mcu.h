#ifndef MCU_H
#define MCU_H

#include <stdint.h>

bool mcu_init();

uint32_t mcu_sysclk_get_freq();
uint64_t mcu_systime_get_current();
void mcu_systime_set_comparator(uint64_t value);

#endif