#include "log.h"

static void (*boot_log_msg)(log_level_t level, const char *msg);
static void (*boot_log_vargs)(log_level_t level, const char *msg, va_list args);

void log_init(boot_iface_t *boot_iface)
{
    boot_log_msg = boot_iface->log_msg;
    boot_log_vargs = boot_iface->log_vargs;
}

void log_msg(log_level_t level, const char *msg)
{
    log_fmt(level, msg, nullptr);
}

void log_fmt(log_level_t level, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    boot_log_vargs(level, msg, args);

    va_end(args);
}