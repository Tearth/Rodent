#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "common.h"

#define TIMER_SIO_REG_BASE 0xd0000000
#define TIMER_SIO_REG_MTIME_CTRL REG((TIMER_SIO_REG_BASE + 0x1a4))
#define TIMER_SIO_REG_MTIME REG((TIMER_SIO_REG_BASE + 0x1b0))
#define TIMER_SIO_REG_MTIMEH REG((TIMER_SIO_REG_BASE + 0x1b4))

void timer_sys_enable();
void timer_sys_disable();
bool timer_sys_is_enabled();

uint64_t timer_sys_get_current();

#endif