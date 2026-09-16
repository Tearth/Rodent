#include "qmi.h"

void qmi_read(void *buf, const void *addr, const size_t size)
{
    memcpy(buf, addr, size);
}