#ifndef BOOT_MCU_H
#define BOOT_MCU_H

#include <stddef.h>
#include <stdint.h>

bool mcu_init();
void uart_send(const char *str);
void flash_read(void *buf, const void *addr, const size_t size);

#endif