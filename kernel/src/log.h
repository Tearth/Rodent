#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include "common/boot.h"
#include "common/log.h"
#include "mcu/mcu.h"

void log_init(boot_iface_t *boot_iface);
void log_msg(log_level_t level, const char *msg);
void log_fmt(log_level_t level, const char *msg, ...);

#endif