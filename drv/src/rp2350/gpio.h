#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "common.h"
#include "reset.h"

#define GPIO_IO_REG_BASE 0x40028000
#define GPIO_IO_REG_CTRL(gpio) REG((GPIO_IO_REG_BASE + 0x04 + gpio * 0x08))
#define GPIO_IO_FUNC_MASK 0x0000001f

#define GPIO_PADS_BANK0_BASE 0x40038000
#define GPIO_PADS_REG_CTRL(gpio) REG((GPIO_PADS_BANK0_BASE + 0x04 + gpio * 0x04))
#define GPIO_PADS_MODE_MASK 0x000000ff

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

void gpio_enable(uint8_t gpio);
void gpio_disable(uint8_t gpio);
bool gpio_is_enabled(uint8_t gpio);
bool gpio_reset();

void gpio_set_func(uint8_t gpio, gpio_func_t func);
void gpio_set_mode(uint8_t gpio, bool input, bool output, bool pull_down, bool pull_up);

#endif