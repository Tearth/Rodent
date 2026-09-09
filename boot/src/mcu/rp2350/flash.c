#include "flash.h"

void flash_read(void *buf, const void *addr, size_t size)
{
    qmi_read(buf, addr, size);
}