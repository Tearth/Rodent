#include "gpio.h"

void gpio_enable(uint8_t gpio)
{
    // Clear ISO (Pad Isolation Control)
    *GPIO_PADS_REG_CTRL(gpio) &= ~(1u << 8);
}

void gpio_disable(uint8_t gpio)
{
    // Set ISO (Pad Isolation Control)
    *GPIO_PADS_REG_CTRL(gpio) |= 1u << 8;
}

bool gpio_is_enabled(uint8_t gpio)
{
    // Read ISO (Pad Isolation Control)
    return (*GPIO_PADS_REG_CTRL(gpio) & (1u << 8)) == 0;
}

bool gpio_reset()
{
    return reset_subsys(RESET_SUBSYS_PADS_BANK0) && reset_subsys(RESET_SUBSYS_IOBANK0);
}

void gpio_set_func(uint8_t gpio, gpio_func_t func)
{
    // Set FUNCSEL
    *GPIO_IO_REG_CTRL(gpio) = (*GPIO_IO_REG_CTRL(gpio) & ~GPIO_IO_FUNC_MASK) | func;
}

void gpio_set_mode(uint8_t gpio, bool input, bool output, bool pull_down, bool pull_up)
{
    // Set PDE (Pull Down Enable), PUE (Pull Up Enable), IE (Input Enable), OD (Output Disable)
    *GPIO_PADS_REG_CTRL(gpio) = (*GPIO_PADS_REG_CTRL(gpio) & ~GPIO_PADS_MODE_MASK) |
        (pull_down << 2) | (pull_up << 3) | (input << 6) | (!output << 7);
}