#ifndef BOOT_LOG_H
#define BOOT_LOG_H

#include <stdarg.h>
#include <string.h>
#include "shared/log.h"
#include "mcu/mcu.h"

#define LOG_BUFFER_SIZE 2048
#define EOL (const char *)nullptr

typedef enum log_mode
{
    LOG_MODE_BUFFER,
    LOG_MODE_UART
} log_mode_t;

void log_msg(log_level_t level, const char *msg);
void log_fmt(log_level_t level, const char *msg, ...);
void log_set_mode(log_mode_t mode);

#endif