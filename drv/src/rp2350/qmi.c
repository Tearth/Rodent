#include "qmi.h"

void qmi_read(void *buf, const void *addr, size_t size)
{
    memcpy(buf, addr, size);
}