#include "uart.h"

static const uart_info_t uart_info[] =
{
    // UART0
    {
        .reg_dr = UART0_REG_DR,
        .reg_fr = UART0_REG_FR,
        .reg_ibrd = UART0_REG_IBRD,
        .reg_fbrd = UART0_REG_FBRD,
        .reg_lcr = UART0_REG_LCR,
        .reg_cr = UART0_REG_CR
    },
    // UART1
    {
        .reg_dr = UART1_REG_DR,
        .reg_fr = UART1_REG_FR,
        .reg_ibrd = UART1_REG_IBRD,
        .reg_fbrd = UART1_REG_FBRD,
        .reg_lcr = UART1_REG_LCR,
        .reg_cr = UART1_REG_CR
    },
};

bool uart_enable(uart_t uart, uint32_t baudrate, uint8_t data_bits, uint8_t stop_bits)
{
    if (!uart_set_baudrate(uart, baudrate))
    {
        return false;
    }

    if (!uart_set_format(uart, data_bits, stop_bits))
    {
        return false;
    }

    // Set UARTEN, TXE, RXE
    *uart_info[uart].reg_cr |= 1u | (1u << 8) | (1u << 9);

    return true;
}

bool uart_disable(uart_t uart)
{
    // Clear UARTEN, TXE, RXE
    *uart_info[uart].reg_cr &= ~(1u | (1u << 8) | (1u << 9));
}

bool uart_is_enabled(uart_t uart)
{
    // Read UARTEN
    return (*uart_info[uart].reg_cr & 1u) == 1;
}

bool uart_reset(uart_t uart)
{
    switch (uart)
    {
        case UART0: return reset_subsys(RESET_SUBSYS_UART0);
        case UART1: return reset_subsys(RESET_SUBSYS_UART1);
        default: return false;
    }
}

bool uart_set_baudrate(uart_t uart, uint32_t baudrate)
{
    uart_info_t *uart_sel = &uart_info[uart];
    uint32_t freq = clk_get_freq(CLK_PERI);

    if (baudrate == 0 || freq == 0)
    {
        return false;
    }

    float divisor = (float)freq / (16 * baudrate);
    uint32_t ibrd = (uint32_t)divisor;
    uint32_t fbrd = (uint32_t)((divisor - (uint32_t)divisor) * 64 + 0.5);

    *uart_sel->reg_ibrd = ibrd;
    *uart_sel->reg_fbrd = fbrd;

    return true;
}

bool uart_set_format(uart_t uart, uint8_t data_bits, uint8_t stop_bits)
{
    if (data_bits < 5 || data_bits > 8)
    {
        return false;
    }

    if (stop_bits < 1 || stop_bits > 2)
    {
        return false;
    }

    // Set WLEN (Word Length), FEN (FIFO Enabled), STP2 (Stop Bits)
    *uart_info[uart].reg_lcr = ((data_bits - 5) << 5) | (1u << 4) | ((stop_bits - 1) << 3);

    return true;
}

uint8_t uart_read_byte(uart_t uart)
{
    uart_info_t *uart_sel = &uart_info[uart];

    // Wait for RXFE (Receive FIFO Empty) to clear
    while ((*uart_sel->reg_fr & (1u << 4)) != 0);

    return *uart_sel->reg_dr;
}

void uart_send_byte(uart_t uart, uint8_t byte)
{
    uart_info_t *uart_sel = &uart_info[uart];

    // Wait for TXFF (Transmit FIFO Full) to clear
    while ((*uart_sel->reg_fr & (1u << 5)) != 0);

    *uart_sel->reg_dr = byte;
}

void uart_send_str(uart_t uart, const char *str)
{
    for (char *ptr = str; *ptr != 0; ptr++)
    {
        uart_send_byte(uart, *ptr);
    }
}