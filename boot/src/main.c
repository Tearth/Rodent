#include <stdlib.h>
#include "cfg/config.h"
#include "fs/fs.h"
#include "hal/hal.h"
#include "log.h"

static bool init_hw();

int main()
{
    if (!init_hw())
    {
        while(1);
    }

    while (1);
}

bool init_hw()
{
    if (!hal_clk_init())
    {
        return false;
    }

    if (!hal_uart_init())
    {
        return false;
    }

    log(LOG_LEVEL_OK, "PicOS Bootloader");
    log(LOG_LEVEL_OK, "Started Clocks");

    hal_clk_info_t clks[8];
    hal_uart_info_t uarts[8];

    for (int i = 0; i < hal_clk_get_list(clks, 8); i++)
    {
        char freq_buf[16];
        char *enabled_buff;

        itoa(clks[i].freq / 1'000'000, freq_buf, 10);

        switch (clks[i].enabled)
        {
            case true: enabled_buff = "active"; break;
            case false: enabled_buff = "inactive"; break;
        }

        log_fmt(LOG_LEVEL_INFO, " ", clks[i].name, " @ ", clks[i].src, " (", freq_buf, " MHz), ", enabled_buff, nullptr);
    }

    log(LOG_LEVEL_OK, "Started UART");

    for (int i = 0; i < hal_uart_get_list(uarts, 8); i++)
    {
        char baudrate_buf[16];
        char data_bits_buf[16];
        char stop_bits_buf[16];
        char *enabled_buff;

        itoa(uarts[i].baudrate, baudrate_buf, 10);
        itoa(uarts[i].data_bits, data_bits_buf, 10);
        itoa(uarts[i].stop_bits, stop_bits_buf, 10);

        switch (uarts[i].enabled)
        {
            case true: enabled_buff = "active"; break;
            case false: enabled_buff = "inactive"; break;
        }

        log_fmt(LOG_LEVEL_INFO, " ", uarts[i].name, " @ ", baudrate_buf, "/", data_bits_buf, "/", stop_bits_buf, ", ", enabled_buff, nullptr);
    }

    return true;
}