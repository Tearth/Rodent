#ifndef MCU_H
#define MCU_H

#include <stddef.h>
#include <stdint.h>

typedef struct clk_info
{
    char name[16];
    char src[16];
    bool enabled;
    uint32_t freq;
} clk_info_t;

typedef struct uart_info
{
    char name[16];
    bool enabled;
    uint32_t baudrate;
    uint8_t data_bits;
    uint8_t stop_bits;
} uart_info_t;

bool clk_init();
size_t clk_get_list(clk_info_t *clks, size_t max_len);

bool uart_init();
void uart_send(const char *str);
size_t uart_get_list(uart_info_t *uarts, size_t max_len);

void flash_read(void *buf, const void *addr, size_t size);

#endif