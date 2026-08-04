#include "gpio.h"

gpio_func_t gpio_get_func(uint8_t gpio)
{
    // Read selected function (FUNCSEL bits)
    return *GPIO_REG_CTRL(gpio) & GPIO_FUNC_MASK;
}

void gpio_set_func(uint8_t gpio, gpio_func_t func)
{
    // Set selected function (FUNCSEL bits)
    *GPIO_REG_CTRL(gpio) = (*GPIO_REG_CTRL(gpio) & ~GPIO_FUNC_MASK) | func;
}