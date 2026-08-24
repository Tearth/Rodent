#include "cpu_rp2350.h"

bool clk_init()
{
    if (!clk_enable(CLK_PERI))
    {
        return false;
    }

    if (!clk_src_enable(CLK_SRC_XOSC))
    {
        return false;
    }

    if (!clk_set_src(CLK_REF, CLK_SRC_XOSC))
    {
        return false;
    }

    if (!clk_set_src(CLK_PERI, CLK_SRC_XOSC))
    {
        return false;
    }

    if (clk_get_src(CLK_SYS) != CLK_SRC_PLL_SYS)
    {
        if (!clk_pll_reset())
        {
            return false;
        }

        if (!clk_pll_enable(CLK_PLL_SYS, 1, 125, 5, 2))
        {
            return false;
        }

        if (!clk_set_src(CLK_SYS, CLK_SRC_PLL_SYS))
        {
            return false;
        }
    }

    return true;
}

size_t clk_get_list(clk_info_t *clks, size_t max_len)
{
    const clk_t clk_types[] = { CLK_REF, CLK_SYS, CLK_PERI };
    size_t count = max_len < 3 ? max_len : 3;

    for (size_t i = 0; i < count; i++)
    {
        const char *clk_name;
        const char *clk_src_name;

        switch (clk_types[i])
        {
            case CLK_REF: clk_name = "CLK_REF"; break;
            case CLK_SYS: clk_name = "CLK_SYS"; break;
            case CLK_PERI: clk_name = "CLK_PERI"; break;
            default: clk_name = "CLK_INVALID"; break;
        }

        switch (clk_get_src(clk_types[i]))
        {
            case CLK_SRC_REF: clk_src_name = "CLK_SRC_REF"; break;
            case CLK_SRC_SYS: clk_src_name = "CLK_SRC_SYS"; break;
            case CLK_SRC_ROSC: clk_src_name = "CLK_SRC_ROSC"; break;
            case CLK_SRC_XOSC: clk_src_name = "CLK_SRC_XOSC"; break;
            case CLK_SRC_LPOSC: clk_src_name = "CLK_SRC_LPOSC"; break;
            case CLK_SRC_PLL_SYS: clk_src_name = "CLK_SRC_PLL_SYS"; break;
            case CLK_SRC_PLL_USB: clk_src_name = "CLK_SRC_PLL_USB"; break;
            default: clk_src_name = "CLK_SRC_INVALID"; break;
        }

        strncpy(clks[i].name, clk_name, sizeof(clks[i].name));
        strncpy(clks[i].src, clk_src_name, sizeof(clks[i].src));

        clks[i].enabled = clk_is_enabled(clk_types[i]);
        clks[i].freq = clk_get_freq(clk_types[i]);
    }

    return count;
}

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

size_t uart_get_list(uart_info_t *uarts, size_t max_len)
{
    const uart_t uart_types[] = { UART0, UART1 };
    size_t count = max_len < 2 ? max_len : 2;

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

void flash_read(void *buf, const void *addr, size_t size)
{
    qmi_read(buf, addr, size);
}