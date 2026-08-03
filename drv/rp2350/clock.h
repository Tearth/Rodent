#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>
#include "common.h"

#define CLK_REG_BASE 0x40010000

#define CLK_REG_REF_CTRL REG((CLK_REG_BASE + 0x30))
#define CLK_REG_REF_DIV REG((CLK_REG_BASE + 0x34))
#define CLK_REG_REF_SEL REG((CLK_REG_BASE + 0x38))
#define CLK_REF_SRC_MASK 0x63
#define CLK_REF_SRC_AUX_MASK 0x03
#define CLK_REF_SRC_ROSC 0x00
#define CLK_REF_SRC_XOSC 0x02
#define CLK_REF_SRC_LPOSC 0x03
#define CLK_REF_AUX_MASK 0x60
#define CLK_REF_SEL_MASK 0x07

#define CLK_REG_SYS_CTRL REG((CLK_REG_BASE + 0x3c))
#define CLK_REG_SYS_DIV REG((CLK_REG_BASE + 0x40))
#define CLK_REG_SYS_SEL REG((CLK_REG_BASE + 0x44))
#define CLK_SYS_SRC_MASK 0xe1
#define CLK_SYS_SRC_REF 0x00
#define CLK_SYS_SRC_ROSC 0x41
#define CLK_SYS_SRC_XOSC 0x61
#define CLK_SYS_AUX_MASK 0xe0
#define CLK_SYS_SEL_MASK 0x01

#define CLK_REG_PERI_CTRL REG((CLK_REG_BASE + 0x48))
#define CLK_REG_PERI_DIV REG((CLK_REG_BASE + 0x4c))
#define CLK_REG_PERI_SEL REG((CLK_REG_BASE + 0x50))
#define CLK_PERI_SRC_MASK 0xe0
#define CLK_PERI_SRC_SYS 0x00
#define CLK_PERI_SRC_ROSC 0x60
#define CLK_PERI_SRC_XOSC 0x80
#define CLK_PERI_SEL_MASK 0x01

typedef enum clk
{
    CLK_REF,
    CLK_SYS,
    CLK_PERI
} clk_t;

typedef enum clk_src
{
    CLK_SRC_REF,
    CLK_SRC_SYS,
    CLK_SRC_ROSC,
    CLK_SRC_XOSC,
    CLK_SRC_LPOSC,
    CLK_SRC_AUX,
    CLK_SRC_INVALID
} clk_src_t;

typedef struct clk_info
{
    uint32_t *reg_ctrl;
    uint32_t *reg_div;
    uint32_t *reg_sel;
    uint32_t src_mask;
    uint32_t aux_mask;
    uint32_t sel_mask;
} clk_info_t;

bool clk_enable(clk_t clk);
bool clk_disable(clk_t clk);
bool clk_is_enabled(clk_t clk);

clk_src_t clk_get_src(clk_t clk);
bool clk_set_src(clk_t clk, clk_src_t src);

#endif