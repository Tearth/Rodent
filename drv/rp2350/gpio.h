#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "common.h"

#define GPIO_REG_BASE 0x40028000
#define GPIO_REG_CTRL(gpio) REG((GPIO_REG_BASE + 0x04 + gpio * 0x10))

#define GPIO_FUNC_MASK 0x0000000f

typedef enum gpio_func
{
    GPIO_FUNC_HSTX = 0,
    GPIO_FUNC_SPI = 1,
    GPIO_FUNC_UART = 2,
    GPIO_FUNC_I2C = 3,
    GPIO_FUNC_PWM = 4,
    GPIO_FUNC_SIO = 5,
    GPIO_FUNC_PIO0 = 6,
    GPIO_FUNC_PIO1 = 7,
    GPIO_FUNC_PIO2 = 8,
    GPIO_FUNC_QMI = 9,
    GPIO_FUNC_CLOCK = 9,
    GPIO_FUNC_TRACEDATA = 9,
    GPIO_FUNC_USB = 10,
    GPIO_FUNC_UART_AUX = 11,
    GPIO_FUNC_NONE = 31
} gpio_func_t;

gpio_func_t gpio_get_func(uint8_t gpio);
void gpio_set_func(uint8_t gpio, gpio_func_t func);

#endif