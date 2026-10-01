#ifndef MCU_UART_H
#define MCU_UART_H

#include <stdint.h>
#include "clk.h"
#include "common.h"
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
    UART1
} uart_t;

typedef struct uart_def
{
    volatile uint32_t *reg_dr;
    volatile uint32_t *reg_fr;
    volatile uint32_t *reg_ibrd;
    volatile uint32_t *reg_fbrd;
    volatile uint32_t *reg_lcr;
    volatile uint32_t *reg_cr;
} uart_def_t;

bool uart_enable(const uart_t uart, const uint32_t baudrate, const uint8_t data_bits, const uint8_t stop_bits);
void uart_disable(const uart_t uart);
bool uart_is_enabled(const uart_t uart);
bool uart_reset(const uart_t uart);

bool uart_set_baudrate(const uart_t uart, const uint32_t baudrate);
uint32_t uart_get_baudrate(const uart_t uart);

bool uart_set_format(const uart_t uart, const uint8_t data_bits, const uint8_t stop_bits);
uint8_t uart_get_data_bits(const uart_t uart);
uint8_t uart_get_stop_bits(const uart_t uart);

uint8_t uart_read_byte(const uart_t uart);
void uart_send_byte(const uart_t uart, const uint8_t byte);
void uart_send_str(const uart_t uart, const char *str);

bool uart_can_read(const uart_t uart);
bool uart_can_write(const uart_t uart);

#endif