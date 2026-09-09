#ifndef BOOT_CLK_H
#define BOOT_CLK_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <rp2350/clk.h>

typedef struct clk_info
{
    char name[16];
    char src[16];
    bool enabled;
    uint32_t freq;
} clk_info_t;

bool clk_init();
size_t clk_get_info(clk_info_t *clks, size_t max_len);

#endif