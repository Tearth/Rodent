#ifndef HAL_H
#define HAL_H

#include <stdint.h>
#include <string.h>

typedef struct hal_clk_info
{
    char name[16];
    char src[16];
    bool enabled;
    uint32_t freq;
} hal_clk_info_t;

typedef struct hal_uart_info
{
    char name[16];
    bool enabled;
    uint32_t baudrate;
    uint8_t data_bits;
    uint8_t stop_bits;
} hal_uart_info_t;

bool hal_clk_init();
uint32_t hal_clk_get_list(hal_clk_info_t *clks, int max_len);

bool hal_uart_init();
void hal_uart_send_str(const char *str);
uint32_t hal_uart_get_list(hal_uart_info_t *uarts, int max_len);

bool hal_systime_init();
uint64_t hal_systime_get_current();

#endif