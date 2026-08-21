#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "common.h"
#include "clock.h"
#include "reset.h"

#define UART0_REG_BASE 0x40070000
#define UART0_REG_DR REG((UART0_REG_BASE + 0x000))
#define UART0_REG_FR REG((UART0_REG_BASE + 0x018))
#define UART0_REG_IBRD REG((UART0_REG_BASE + 0x024))
#define UART0_REG_FBRD REG((UART0_REG_BASE + 0x028))
#define UART0_REG_LCR REG((UART0_REG_BASE + 0x02c))
#define UART0_REG_CR REG((UART0_REG_BASE + 0x030))

#define UART1_REG_BASE 0x40078000
#define UART1_REG_DR REG((UART1_REG_BASE + 0x000))
#define UART1_REG_FR REG((UART1_REG_BASE + 0x018))
#define UART1_REG_IBRD REG((UART1_REG_BASE + 0x024))
#define UART1_REG_FBRD REG((UART1_REG_BASE + 0x028))
#define UART1_REG_LCR REG((UART1_REG_BASE + 0x02c))
#define UART1_REG_CR REG((UART1_REG_BASE + 0x030))

typedef enum uart
{
    UART0,
    UART1,
    UART_INVALID
} uart_t;

typedef struct uart_info
{
    volatile uint32_t *reg_dr;
    volatile uint32_t *reg_fr;
    volatile uint32_t *reg_ibrd;
    volatile uint32_t *reg_fbrd;
    volatile uint32_t *reg_lcr;
    volatile uint32_t *reg_cr;
} uart_info_t;

bool uart_enable(uart_t uart, uint32_t baudrate, uint8_t data_bits, uint8_t stop_bits);
bool uart_disable(uart_t uart);
bool uart_is_enabled(uart_t uart);
bool uart_reset(uart_t uart);

bool uart_set_baudrate(uart_t uart, uint32_t baudrate);
uint32_t uart_get_baudrate(uart_t uart);

bool uart_set_format(uart_t uart, uint8_t data_bits, uint8_t stop_bits);
uint8_t uart_get_data_bits(uart_t uart);
uint8_t uart_get_stop_bits(uart_t uart);

uint8_t uart_read_byte(uart_t uart);
void uart_send_byte(uart_t uart, uint8_t byte);
void uart_send_str(uart_t uart, const char *str);

#endif