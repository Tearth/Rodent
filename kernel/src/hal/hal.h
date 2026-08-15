#ifndef HAL_H
#define HAL_H

#include <stdint.h>

void hal_uart_send_str(const char *str);

bool hal_systime_init();
uint64_t hal_systime_get_current();

#endif