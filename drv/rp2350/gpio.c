#include "gpio.h"

gpio_func_t gpio_get_func(uint8_t gpio)
{
    // Read FUNCSEL
    return *GPIO_REG_CTRL(gpio) & GPIO_FUNC_MASK;
}

void gpio_set_func(uint8_t gpio, gpio_func_t func)
{
    // Set FUNCSEL
    *GPIO_REG_CTRL(gpio) = (*GPIO_REG_CTRL(gpio) & ~GPIO_FUNC_MASK) | func;
}