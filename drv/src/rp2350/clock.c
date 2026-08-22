#include "clock.h"

static const clk_info_t clk_info[] =
{
    // CLK_REF
    {
        .reg_ctrl = CLK_REF_REG_CTRL,
        .reg_sel = CLK_REF_REG_SEL,
        .src_mask = CLK_REF_SRC_MASK,
        .aux_mask = CLK_REF_AUX_MASK,
        .sel_mask = CLK_REF_SEL_MASK
    },
    // CLK_SYS
    {
        .reg_ctrl = CLK_SYS_REG_CTRL,
        .reg_sel = CLK_SYS_REG_SEL,
        .src_mask = CLK_SYS_SRC_MASK,
        .aux_mask = CLK_SYS_AUX_MASK,
        .sel_mask = CLK_SYS_SEL_MASK
    },
    // CLK_PERI
    {
        .reg_ctrl = CLK_PERI_REG_CTRL,
        .reg_sel = CLK_PERI_REG_SEL,
        .src_mask = CLK_PERI_SRC_MASK,
        .aux_mask = CLK_PERI_AUX_MASK,
        .sel_mask = CLK_PERI_SEL_MASK
    }
};

static const clk_src_info_t clk_src_info[] =
{
    // CLK_SRC_REF
    { },
    // CLK_SRC_SYS
    { },
    // CLK_SRC_ROSC
    {
        .reg_ctrl = CLK_SRC_ROSC_REG_CTRL,
        .reg_status = CLK_SRC_ROSC_REG_STATUS
    },
    // CLK_SRC_XOSC
    {
        .reg_ctrl = CLK_SRC_XOSC_REG_CTRL,
        .reg_status = CLK_SRC_XOSC_REG_STATUS
    },
    // CLK_SRC_LPOSC
    { }
};

static const clk_pll_info_t clk_pll_info[] =
{
    // CLK_PLL_SYS
    {
        .reg_cs = CLK_PLL_SYS_REG_CS,
        .reg_pwr = CLK_PLL_SYS_REG_PWR,
        .reg_fbdiv = CLK_PLL_SYS_REG_FBDIV,
        .reg_prim = CLK_PLL_SYS_REG_PRIM
    },
    // CLK_PLL_USB
    {
        .reg_cs = CLK_PLL_USB_REG_CS,
        .reg_pwr = CLK_PLL_USB_REG_PWR,
        .reg_fbdiv = CLK_PLL_USB_REG_FBDIV,
        .reg_prim = CLK_PLL_USB_REG_PRIM
    }
};

static bool clk_set_src_internal(clk_t clk, clk_src_t src, uint32_t mask);
static bool clk_is_aux_ready_internal(clk_t clk);

bool clk_enable(clk_t clk)
{
    const clk_info_t* clk_sel = &clk_info[clk];

    // Set ENABLE
    *clk_sel->reg_ctrl |= 1u << 11;

    // Wait for ENABLED to set
    WAIT(!clk_is_enabled(clk), 100);

    return true;
}

bool clk_disable(clk_t clk)
{
    const clk_info_t* clk_sel = &clk_info[clk];

    // Clear ENABLE
    *clk_sel->reg_ctrl &= ~(1u << 11);

    // Wait for ENABLED to clear
    WAIT(clk_is_enabled(clk), 100);

    return true;
}

bool clk_is_enabled(clk_t clk)
{
    if (clk == CLK_REF || clk == CLK_SYS)
    {
        return true;
    }

    // Read ENABLED
    return (*clk_info[clk].reg_ctrl & (1u << 28)) != 0;
}

bool clk_src_enable(clk_src_t src)
{
    const clk_src_info_t *clk_src_sel = &clk_src_info[src];

    if (clk_src_sel->reg_ctrl == nullptr)
    {
        return false;
    }

    // Set ENABLE, code 0xfab enables clock source
    *clk_src_sel->reg_ctrl = (*clk_src_sel->reg_ctrl & ~CLK_SRC_MASK) | (0xfab << 12);

    // Wait for ENABLED and STABLE to set, stabilization might take a bit of time so timeout is higher than usual
    WAIT(!clk_src_is_enabled(src) || !clk_src_is_stable(src), 100000);

    return true;
}

bool clk_src_disable(clk_src_t src)
{
    const clk_src_info_t *clk_src_sel = &clk_src_info[src];

    if (clk_src_sel->reg_ctrl == nullptr)
    {
        return false;
    }

    // Set ENABLE, code 0xd1e disables clock source
    *clk_src_sel->reg_ctrl = (*clk_src_sel->reg_ctrl & ~CLK_SRC_MASK) | (0xd1e << 12);

    // Wait for ENABLED to clear
    WAIT(clk_src_is_enabled(src), 100);

    return true;
}

bool clk_src_is_enabled(clk_src_t src)
{
    const clk_src_info_t *clk_src_sel = &clk_src_info[src];

    if (clk_src_sel->reg_ctrl == nullptr)
    {
        return false;
    }

    // Read ENABLED
    return (*clk_src_sel->reg_status & (1u << 12)) != 0;
}

bool clk_src_is_stable(clk_src_t src)
{
    const clk_src_info_t *clk_src_sel = &clk_src_info[src];

    if (clk_src_sel->reg_ctrl == nullptr)
    {
        return false;
    }

    // Read STABLE
    return (*clk_src_sel->reg_status & (1u << 31)) != 0;
}

clk_src_t clk_get_src(clk_t clk)
{
    switch (clk)
    {
        case CLK_REF:
            switch (*clk_info[CLK_REF].reg_ctrl & CLK_REF_SRC_MASK)
            {
                case CLK_REF_SRC_ROSC: return CLK_SRC_ROSC;
                case CLK_REF_SRC_XOSC: return CLK_SRC_XOSC;
                case CLK_REF_SRC_LPOSC: return CLK_SRC_LPOSC;
                default: return CLK_SRC_INVALID;
            }
            break;
        case CLK_SYS:
            switch (*clk_info[CLK_SYS].reg_ctrl & CLK_SYS_SRC_MASK)
            {
                case CLK_SYS_SRC_REF: return CLK_SRC_REF;
                case CLK_SYS_SRC_PLL_SYS: return CLK_SRC_PLL_SYS;
                case CLK_SYS_SRC_PLL_USB: return CLK_SRC_PLL_USB;
                case CLK_SYS_SRC_ROSC: return CLK_SRC_ROSC;
                case CLK_SYS_SRC_XOSC: return CLK_SRC_XOSC;
                default: return CLK_SRC_INVALID;
            }
            break;
        case CLK_PERI:
            switch (*clk_info[CLK_PERI].reg_ctrl & CLK_PERI_SRC_MASK)
            {
                case CLK_PERI_SRC_SYS: return CLK_SRC_SYS;
                case CLK_PERI_SRC_PLL_SYS: return CLK_SRC_PLL_SYS;
                case CLK_PERI_SRC_PLL_USB: return CLK_SRC_PLL_USB;
                case CLK_PERI_SRC_ROSC: return CLK_SRC_ROSC;
                case CLK_PERI_SRC_XOSC: return CLK_SRC_XOSC;
                default: return CLK_SRC_INVALID;
            }
            break;
        default: return CLK_SRC_INVALID;
    }
}

bool clk_set_src(clk_t clk, clk_src_t src)
{
    const clk_info_t *clk_sel = &clk_info[clk];
    const clk_src_t src_old = clk_get_src(clk);

    if (src == src_old)
    {
        return true;
    }

    // Check if clock source switch can be done using only glitchless mux
    bool glitchless = false;
    switch (clk)
    {
        case CLK_REF:
            glitchless = src == CLK_SRC_ROSC || src == CLK_SRC_XOSC || src == CLK_SRC_LPOSC;
            break;
        case CLK_SYS:
            glitchless = src == CLK_SRC_REF;
            break;
        default: break;
    }

    if (glitchless)
    {
        if (!clk_set_src_internal(clk, src, clk_sel->src_mask & ~clk_sel->aux_mask))
        {
            return false;
        }
    }
    else
    {
        bool result = false;

        // Switch away from aux to temporary clock source, so it can be reconfigured without glitches
        switch (clk)
        {
            case CLK_REF: result = clk_set_src_internal(clk, CLK_SRC_LPOSC, (clk_sel->src_mask & ~clk_sel->aux_mask)); break;
            case CLK_SYS: result = clk_set_src_internal(clk, CLK_SRC_REF, (clk_sel->src_mask & ~clk_sel->aux_mask)); break;
            default: result = true;
        }

        if (!result)
        {
            return false;
        }

        // Change clock source of aux, leave glitchless mux for now
        if (!clk_set_src_internal(clk, src, (clk_sel->src_mask & clk_sel->aux_mask)))
        {
            return false;
        }

        // Wait for SELECTED register to indicate that a new clock source is used
        WAIT(!clk_is_aux_ready_internal(clk), 100);

        // Change clock source for both aux and glitchless mux
        if (!clk_set_src_internal(clk, src, clk_sel->src_mask))
        {
            return false;
        }
    }

    return true;
}

static bool clk_set_src_internal(clk_t clk, clk_src_t src, uint32_t mask)
{
    const clk_info_t* clk_sel = &clk_info[clk];
    uint32_t val = 0;

    switch (clk)
    {
        case CLK_REF:
            switch (src)
            {
                case CLK_SRC_ROSC: val = CLK_REF_SRC_ROSC; break;
                case CLK_SRC_XOSC: val = CLK_REF_SRC_XOSC; break;
                case CLK_SRC_LPOSC: val = CLK_REF_SRC_LPOSC; break;
                default: return false;
            }
            break;
        case CLK_SYS:
            switch (src)
            {
                case CLK_SRC_REF: val = CLK_SYS_SRC_REF; break;
                case CLK_SRC_ROSC: val = CLK_SYS_SRC_ROSC; break;
                case CLK_SRC_XOSC: val = CLK_SYS_SRC_XOSC; break;
                case CLK_SRC_PLL_SYS: val = CLK_SYS_SRC_PLL_SYS; break;
                case CLK_SRC_PLL_USB: val = CLK_SYS_SRC_PLL_USB; break;
                default: return false;
            }
            break;
        case CLK_PERI:
            switch (src)
            {
                case CLK_SRC_SYS: val = CLK_PERI_SRC_SYS; break;
                case CLK_SRC_ROSC: val = CLK_PERI_SRC_ROSC; break;
                case CLK_SRC_XOSC: val = CLK_PERI_SRC_XOSC; break;
                case CLK_SRC_PLL_SYS: val = CLK_PERI_SRC_PLL_SYS; break;
                case CLK_SRC_PLL_USB: val = CLK_PERI_SRC_PLL_USB; break;
                default: return false;
            }
            break;
        default: return false;
    }

    // Set SRC and/or AUXSRC
    *clk_sel->reg_ctrl = (*clk_sel->reg_ctrl & ~mask) | (val & mask);

    return true;
}

static bool clk_is_aux_ready_internal(clk_t clk)
{
    const clk_info_t* clk_sel = &clk_info[clk];

    // Read masked SELECTED register, any set bit indicates aux uses this clock source
    return (*clk_sel->reg_sel & clk_sel->sel_mask) != 0;
}

bool clk_pll_enable(clk_pll_t pll, uint8_t refdiv, uint16_t fbdiv, uint8_t pdiv1, uint8_t pdiv2)
{
    const clk_pll_info_t* pll_sel = &clk_pll_info[pll];

    // Set REFDIV
    *pll_sel->reg_cs = (*pll_sel->reg_cs & ~0x3f) | refdiv;

    // Set FBDIV
    *pll_sel->reg_fbdiv = (*pll_sel->reg_fbdiv & ~0xfff) | fbdiv;

    // Clear PD (PLL Powerdown), VCOPD (PLL VCO Powerdown)
    *pll_sel->reg_pwr &= ~(1u | (1u << 5));

    // Wait for LOCK to clear
    WAIT((*pll_sel->reg_cs & (1u << 30)) != 0, 100);

    // Set POSTDIV1
    *pll_sel->reg_prim = (*pll_sel->reg_prim & ~0x70000) | (pdiv1 << 16);

    // Set POSTDIV2
    *pll_sel->reg_prim = (*pll_sel->reg_prim & ~0x7000) | (pdiv2 << 12);

    // Clear POSTDIVPD (PLL Post Divider Powerdown)
    *pll_sel->reg_pwr &= ~(1u << 3);

    return true;
}

void clk_pll_disable(clk_pll_t pll)
{
    // Set PD (PLL Powerdown), POSTDIVPD (PLL Post Divider Powerdown), VCOPD (PLL VCO Powerdown)
    *clk_pll_info[pll].reg_pwr |= 1u | (1u << 3) | (1u << 5);
}

bool clk_pll_is_enabled(clk_pll_t pll)
{
    // Read PD (PLL Powerdown)
    return (*clk_pll_info[pll].reg_pwr & 1u) == 0;
}

bool clk_pll_reset()
{
    return reset_subsys(RESET_SUBSYS_PLL_SYS);
}

uint32_t clk_get_freq(clk_t clk)
{
    switch (clk_get_src(clk))
    {
        case CLK_SRC_REF: return clk_get_freq(CLK_REF);
        case CLK_SRC_SYS: return clk_get_freq(CLK_SYS);
        case CLK_SRC_ROSC: return CLK_SRC_ROSC_FREQ;
        case CLK_SRC_XOSC: return CLK_SRC_XOSC_FREQ;
        case CLK_SRC_LPOSC: return CLK_SRC_LPOSC_FREQ;
        case CLK_SRC_PLL_SYS: return clk_measure_freq(clk);
        case CLK_SRC_PLL_USB: return clk_measure_freq(clk);
        default: return 0;
    }
}

uint32_t clk_measure_freq(clk_t clk)
{
    uint8_t src = 0;

    switch (clk)
    {
        case CLK_REF: src = 0x08; break;
        case CLK_SYS: src = 0x09; break;
        case CLK_PERI: src = 0x0a; break;
        default: return 0;
    }

    *CLK_FC0_REG_REF = (*CLK_FC0_REG_REF & ~0xfffff) | (clk_get_freq(CLK_REF) / 1000);
    *CLK_FC0_REG_MIN = (*CLK_FC0_REG_MIN & ~0xffffff) | 0;
    *CLK_FC0_REG_MAX = (*CLK_FC0_REG_MAX & ~0xffffff) | 0x1ffffff;
    *CLK_FC0_REG_SRC = (*CLK_FC0_REG_SRC & ~0xff) | src;

    // Wait for DONE to set
    while ((*CLK_FC0_REG_STATUS & (1u << 4)) == 0);

    // Read KHZ
    return (*CLK_FC0_REG_RESULT >> 5) * 1000;
}