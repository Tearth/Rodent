#include "fifo.h"

uint32_t fifo_read()
{
    // Wait for VLD to clear
    while (!fifo_can_read());

    return *FIFO_SIO_REG_RD;
}

void fifo_write(uint32_t data)
{
    // Wait for RDY to clear
    while (!fifo_can_write());

    *FIFO_SIO_REG_WR = data;
}

bool fifo_can_read()
{
    // Read VLD
    return (*FIFO_SIO_REG_ST & (1u << 0)) != 0;
}

bool fifo_can_write()
{
    // Read RDY
    return (*FIFO_SIO_REG_ST & (1u << 1)) != 0;
}