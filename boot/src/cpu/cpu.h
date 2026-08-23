#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <string.h>

typedef struct clk_dinfo
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
uint32_t clk_get_list(clk_info_t *clks, int max_len);

bool uart_init();
void uart_send(const char *str);
uint32_t uart_get_list(uart_info_t *uarts, int max_len);

#endif