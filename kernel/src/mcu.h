#ifndef KERNEL_MCU_H
#define KERNEL_MCU_H

#include <stdint.h>

bool mcu_init(const bool core0);

uint32_t mcu_sysclk_get_freq();
uint64_t mcu_systime_get_current();
void mcu_systime_set_comparator(const uint64_t value);
void uart_send(const char *str);

#endif