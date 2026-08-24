#ifndef QMI_H
#define QMI_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

void qmi_read(void *buf, const void *addr, size_t size);

#endif