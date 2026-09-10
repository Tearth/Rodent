#ifndef KERNEL_LOG_H
#define KERNEL_LOG_H

#include <stdarg.h>
#include "shared/boot.h"
#include "shared/log.h"
#include "mcu/mcu.h"

#define EOL (const char *)nullptr

void log_msg(log_level_t level, const char *msg);
void log_fmt(log_level_t level, const char *msg, ...);

#endif