#ifndef MCU_FIFO_H
#define MCU_FIFO_H

#include <stdint.h>
#include "common.h"

#define FIFO_SIO_REG_BASE 0xd0000000
#define FIFO_SIO_REG_ST REG((FIFO_SIO_REG_BASE + 0x050))
#define FIFO_SIO_REG_WR REG((FIFO_SIO_REG_BASE + 0x054))
#define FIFO_SIO_REG_RD REG((FIFO_SIO_REG_BASE + 0x058))

uint32_t fifo_read();
void fifo_write(uint32_t data);

bool fifo_can_read();
bool fifo_can_write();

#endif