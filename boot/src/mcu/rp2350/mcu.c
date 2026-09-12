#include <stdlib.h>
#include "clk.h"
#include "flash.h"
#include "log.h"
#include "mcu.h"
#include "uart.h"

static bool log_clk_info();
static bool log_uart_info();

bool mcu_init()
{
    if (!clk_init())
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to initialize clock"), false;
    }

    log_msg(LOG_LEVEL_OK, "Started clocks");
    log_clk_info();

    if (!uart_init())
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to initialize UART"), false;
    }

    log_msg(LOG_LEVEL_OK, "Started UART");
    log_uart_info();

    return true;
}

static bool log_clk_info()
{
    clk_info_t clks[8];

    for (size_t i = 0; i < clk_get_info(clks, 8); i++)
    {
        char freq_buf[16];
        const char *enabled_buf;

        itoa(clks[i].freq / 1'000'000, freq_buf, 10);

        switch (clks[i].enabled)
        {
            case true: enabled_buf = "active"; break;
            case false: enabled_buf = "inactive"; break;
        }

        log_fmt(LOG_LEVEL_INFO, " ", clks[i].name, " @ ", clks[i].src, " (", freq_buf, " MHz), ", enabled_buf, EOL);
    }

    return true;
}

static bool log_uart_info()
{
    uart_info_t uarts[8];

    for (size_t i = 0; i < uart_get_info(uarts, 8); i++)
    {
        char baudrate_buf[16];
        char data_bits_buf[16];
        char stop_bits_buf[16];
        const char *enabled_buf;

        itoa(uarts[i].baudrate, baudrate_buf, 10);
        itoa(uarts[i].data_bits, data_bits_buf, 10);
        itoa(uarts[i].stop_bits, stop_bits_buf, 10);

        switch (uarts[i].enabled)
        {
            case true: enabled_buf = "active"; break;
            case false: enabled_buf = "inactive"; break;
        }

        log_fmt(LOG_LEVEL_INFO, " ", uarts[i].name, " @ ", baudrate_buf, "/", data_bits_buf, "/", stop_bits_buf, ", ", enabled_buf, EOL);
    }

    return true;
}