#ifndef BOOT_FLASH_H
#define BOOT_FLASH_H

#include <stddef.h>
#include <rp2350/qmi.h>

void flash_read(void *buf, const void *addr, const size_t size);

#endif