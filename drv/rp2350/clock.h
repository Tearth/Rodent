#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>
#include "common.h"

#define CLK_REG_BASE 0x40010000

#define CLK_REF_REG_CTRL REG((CLK_REG_BASE + 0x30))
#define CLK_REF_REG_SEL REG((CLK_REG_BASE + 0x38))
#define CLK_REF_SRC_MASK 0x63
#define CLK_REF_SRC_AUX_MASK 0x03
#define CLK_REF_SRC_ROSC 0x00
#define CLK_REF_SRC_XOSC 0x02
#define CLK_REF_SRC_LPOSC 0x03
#define CLK_REF_AUX_MASK 0x60
#define CLK_REF_SEL_MASK 0x07

#define CLK_SYS_REG_CTRL REG((CLK_REG_BASE + 0x3c))
#define CLK_SYS_REG_SEL REG((CLK_REG_BASE + 0x44))
#define CLK_SYS_SRC_MASK 0xe1
#define CLK_SYS_SRC_REF 0x00
#define CLK_SYS_SRC_ROSC 0x41
#define CLK_SYS_SRC_XOSC 0x61
#define CLK_SYS_AUX_MASK 0xe0
#define CLK_SYS_SEL_MASK 0x01

#define CLK_PERI_REG_CTRL REG((CLK_REG_BASE + 0x48))
#define CLK_PERI_REG_SEL REG((CLK_REG_BASE + 0x50))
#define CLK_PERI_SRC_MASK 0xe0
#define CLK_PERI_SRC_SYS 0x00
#define CLK_PERI_SRC_ROSC 0x60
#define CLK_PERI_SRC_XOSC 0x80
#define CLK_PERI_AUX_MASK 0xe0
#define CLK_PERI_SEL_MASK 0x01

#define CLK_SRC_MASK 0xfff000

#define CLK_FC0_REG_REF REG((CLK_REG_BASE + 0x8c))
#define CLK_FC0_REG_MIN REG((CLK_REG_BASE + 0x90))
#define CLK_FC0_REG_MAX REG((CLK_REG_BASE + 0x94))
#define CLK_FC0_REG_SRC REG((CLK_REG_BASE + 0xa0))
#define CLK_FC0_REG_STATUS REG((CLK_REG_BASE + 0xa4))
#define CLK_FC0_REG_RESULT REG((CLK_REG_BASE + 0xa8))

#define CLK_SRC_ROSC_REG_BASE 0x400e8000
#define CLK_SRC_ROSC_REG_CTRL REG((CLK_SRC_ROSC_REG_BASE + 0x00))
#define CLK_SRC_ROSC_REG_STATUS REG((CLK_SRC_ROSC_REG_BASE + 0x1c))

#define CLK_SRC_XOSC_REG_BASE 0x40048000
#define CLK_SRC_XOSC_REG_CTRL REG((CLK_SRC_XOSC_REG_BASE + 0x00))
#define CLK_SRC_XOSC_REG_STATUS REG((CLK_SRC_XOSC_REG_BASE + 0x04))

#define CLK_SRC_ROSC_LPOSC 32'768
#define CLK_SRC_ROSC_FREQ 11'000'000
#define CLK_SRC_XOSC_FREQ 12'000'000

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
    CLK_SRC_INVALID
} clk_src_t;

typedef struct clk_info
{
    volatile uint32_t *reg_ctrl;
    volatile uint32_t *reg_sel;
    uint32_t src_mask;
    uint32_t aux_mask;
    uint32_t sel_mask;
} clk_info_t;

typedef struct clk_src_info
{
    volatile uint32_t *reg_ctrl;
    volatile uint32_t *reg_status;
} clk_src_info_t;

bool clk_enable(clk_t clk);
bool clk_disable(clk_t clk);
bool clk_is_enabled(clk_t clk);

bool clk_src_enable(clk_src_t src);
bool clk_src_disable(clk_src_t src);
bool clk_src_is_enabled(clk_src_t src);
bool clk_src_is_stable(clk_src_t src);

clk_src_t clk_get_src(clk_t clk);
bool clk_set_src(clk_t clk, clk_src_t src);

uint32_t clk_get_freq(clk_t clk);
uint32_t clk_measure_freq(clk_t clk);

#endif