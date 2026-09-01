#include <stdlib.h>
#include "rp2350/uart.h"

int main()
{
    uint32_t i = 0;
    char buf[16] = {};

    while(1)
    {
        itoa(i, buf, 10);
        uart_send_str(0, "App 2: ");
        uart_send_str(0, buf);
        uart_send_str(0, "\r\n");

        i++;
    }
}