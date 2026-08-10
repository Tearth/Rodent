#include "log.h"

void log(log_level_t level, const char *msg)
{
    log_fmt(level, msg, nullptr);
}

void log_fmt(log_level_t level, const char *msg, ...)
{
    switch (level)
    {
        case LOG_LEVEL_OK: hal_uart_send_str("[  \033[32mOK\033[0m  ] "); break;
        case LOG_LEVEL_INFO: hal_uart_send_str("[ \033[0mINFO\033[0m ] "); break;
        case LOG_LEVEL_WARN: hal_uart_send_str("[ \033[33mWARN\033[0m ] "); break;
        case LOG_LEVEL_FAIL: hal_uart_send_str("[ \033[31mFAIL\033[0m ] "); break;
    }

    hal_uart_send_str(msg);

    va_list args;
    va_start(args, msg);
    const char *chunk;

    while (chunk = va_arg(args, const char *), chunk != nullptr)
    {
        hal_uart_send_str(chunk);
    }

    hal_uart_send_str("\r\n");
}