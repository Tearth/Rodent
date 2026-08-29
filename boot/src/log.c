#include "log.h"

void log_msg(log_level_t level, const char *msg)
{
    log_fmt(level, msg, nullptr);
}

void log_fmt(log_level_t level, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    log_vargs(level, msg, args);

    va_end(args);
}

void log_vargs(log_level_t level, const char *msg, va_list args)
{
    const char *chunk;

    switch (level)
    {
        case LOG_LEVEL_OK: uart_send("[  \033[32mOK\033[0m  ] "); break;
        case LOG_LEVEL_INFO: uart_send("[ \033[0mINFO\033[0m ] "); break;
        case LOG_LEVEL_WARN: uart_send("[ \033[33mWARN\033[0m ] "); break;
        case LOG_LEVEL_FAIL: uart_send("[ \033[31mFAIL\033[0m ] "); break;
    }

    uart_send(msg);

    while ((chunk = va_arg(args, const char *)) != nullptr)
    {
        uart_send(chunk);
    }

    uart_send("\r\n");
}