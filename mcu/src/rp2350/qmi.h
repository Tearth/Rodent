#ifndef MCU_QMI_H
#define MCU_QMI_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

void qmi_read(void *buf, const void *addr, const size_t size);

#endif