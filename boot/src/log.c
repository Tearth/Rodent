#include "log.h"

log_mode_t log_mode = LOG_MODE_BUFFER;
char buffer[LOG_BUFFER_SIZE];
size_t buffer_pos = 0;

static void log_internal(const char *msg);
static void log_flush();

void log_msg(const log_level_t level, const char *msg)
{
    log_fmt(level, msg, EOL);
}

void log_fmt(const log_level_t level, const char *msg, ...)
{
    switch (level)
    {
        case LOG_LEVEL_OK: log_internal("[  \033[32mOK\033[0m  ] "); break;
        case LOG_LEVEL_INFO: log_internal("[ \033[0mINFO\033[0m ] "); break;
        case LOG_LEVEL_WARN: log_internal("[ \033[33mWARN\033[0m ] "); break;
        case LOG_LEVEL_FAIL: log_internal("[ \033[31mFAIL\033[0m ] "); break;
    }

    log_internal(msg);

    va_list args;
    va_start(args, msg);
    const char *chunk;

    while ((chunk = va_arg(args, const char *)) != EOL)
    {
        log_internal(chunk);
    }

    log_internal("\r\n");

    va_end(args);
}

void log_set_mode(const log_mode_t mode)
{
    log_mode = mode;
    log_flush();
}

static void log_internal(const char *msg)
{
    switch (log_mode)
    {
        case LOG_MODE_BUFFER:
        {
            if (buffer_pos < LOG_BUFFER_SIZE)
            {
                for (size_t msg_i = 0; buffer_pos < LOG_BUFFER_SIZE; buffer_pos++, msg_i++)
                {
                    buffer[buffer_pos] = msg[msg_i];
                    if (msg[msg_i] == 0)
                    {
                        break;
                    }
                }
            }

            break;
        }
        case LOG_MODE_UART:
        {
            uart_send(msg);
            break;
        }
    }
}

static void log_flush()
{
    if (buffer_pos > 0)
    {
        uart_send(buffer);
        buffer_pos = 0;
    }
}