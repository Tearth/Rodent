#ifndef BOOT_MCU_RP2350_UART_H
#define BOOT_MCU_RP2350_UART_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <rp2350/gpio.h>
#include <rp2350/uart.h>
#include <shared/macro.h>

typedef struct uart_info
{
    char name[16];
    bool enabled;
    uint32_t baudrate;
    uint8_t data_bits;
    uint8_t stop_bits;
} uart_info_t;

bool uart_init();
void uart_send(const char *str);
size_t uart_get_info(uart_info_t *uarts, const size_t len);

#endif