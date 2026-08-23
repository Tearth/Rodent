#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include "cpu/cpu.h"

typedef enum log_level
{
    LOG_LEVEL_OK,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_FAIL
} log_level_t;

void log_msg(log_level_t level, const char *msg);
void log_fmt(log_level_t level, const char *msg, ...);

#endif