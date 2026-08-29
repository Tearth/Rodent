#ifndef COMMON_BOOT_H
#define COMMON_BOOT_H

#include <stdarg.h>
#include "log.h"

typedef struct boot_data
{
    void (*log_msg)(log_level_t level, const char *msg);
    void (*log_vargs)(log_level_t level, const char *msg, va_list args);
} boot_data_t;

#endif