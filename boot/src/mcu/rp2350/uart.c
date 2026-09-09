#include "uart.h"

bool uart_init()
{
    if (!gpio_reset())
    {
        return false;
    }

    if (!uart_reset(UART0))
    {
        return false;
    }

    gpio_set_func(0, GPIO_FUNC_UART);
    gpio_set_func(1, GPIO_FUNC_UART);

    gpio_set_mode(0, false, true, false, false);
    gpio_set_mode(1, true, false, false, false);

    gpio_enable(0);
    gpio_enable(1);

    if (!uart_enable(UART0, 115200, 8, 1))
    {
        return false;
    }

    return true;
}

void uart_send(const char *str)
{
    uart_send_str(UART0, str);
}

size_t uart_get_info(uart_info_t *uarts, size_t len)
{
    const uart_t uart_types[] = { UART0, UART1 };
    size_t count = len < 2 ? len : 2;

    for (size_t i = 0; i < count; i++)
    {
        const char *uart_name;

        switch (uart_types[i])
        {
            case UART0: uart_name = "UART0"; break;
            case UART1: uart_name = "UART1"; break;
            default: uart_name = "UART_INVALID"; break;
        }

        strncpy(uarts[i].name, uart_name, sizeof(uarts[i].name));

        uarts[i].enabled = uart_is_enabled(uart_types[i]);
        uarts[i].baudrate = uart_get_baudrate(uart_types[i]);
        uarts[i].data_bits = uart_get_data_bits(uart_types[i]);
        uarts[i].stop_bits = uart_get_stop_bits(uart_types[i]);
    }

    return count;
}