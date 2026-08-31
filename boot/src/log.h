#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include "shared/log.h"
#include "mcu/mcu.h"

void log_msg(log_level_t level, const char *msg);
void log_fmt(log_level_t level, const char *msg, ...);
void log_vargs(log_level_t level, const char *msg, va_list args);

#endif