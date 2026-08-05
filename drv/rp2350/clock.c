#include "clock.h"

static const clk_info_t clk_info[] =
{
    // CLK_REF
    {
        .reg_ctrl = CLK_REG_REF_CTRL,
        .reg_sel = CLK_REG_REF_SEL,
        .src_mask = CLK_REF_SRC_MASK,
        .aux_mask = CLK_REF_AUX_MASK,
        .sel_mask = CLK_REF_SEL_MASK
    },
    // CLK_SYS
    {
        .reg_ctrl = CLK_REG_SYS_CTRL,
        .reg_sel = CLK_REG_SYS_SEL,
        .src_mask = CLK_SYS_SRC_MASK,
        .aux_mask = CLK_SYS_AUX_MASK,
        .sel_mask = CLK_SYS_SEL_MASK
    },
    // CLK_PERI
    {
        .reg_ctrl = CLK_REG_PERI_CTRL,
        .reg_sel = CLK_REG_PERI_SEL,
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

static bool clk_set_src_internal(clk_t clk, clk_src_t src, uint32_t mask);
static bool clk_is_aux_ready_internal(clk_t clk);

bool clk_enable(clk_t clk)
{
    clk_info_t* clk_sel = &clk_info[clk];

    // Set ENABLE
    *clk_sel->reg_ctrl |= 1u << 11;

    // Wait for ENABLED to set
    WAIT(!clk_is_enabled(clk), 100);

    return true;
}

bool clk_disable(clk_t clk)
{
    clk_info_t* clk_sel = &clk_info[clk];

    // Clear ENABLE
    *clk_sel->reg_ctrl &= ~(1u << 11);

    // Wait for ENABLED to clear
    WAIT(clk_is_enabled(clk), 100);

    return true;
}

bool clk_is_enabled(clk_t clk)
{
    // Read ENABLED
    return (*clk_info[clk].reg_ctrl & (1u << 28)) != 0;
}

bool clk_src_enable(clk_src_t src)
{
    clk_src_info_t *clk_src_sel = &clk_src_info[src];

    if (clk_src_sel->reg_ctrl == nullptr)
    {
        return false;
    }

    // Set ENABLE, code 0xfab enables clock source
    *clk_src_sel->reg_ctrl = (*clk_src_sel->reg_ctrl & ~CLK_SRC_MASK) | (0xfab << 12);

    // Wait for ENABLED and STABLE to set, stabilization might take a bit of time so timeout is higher than usual
    WAIT(!clk_src_is_enabled(src) && !clk_src_is_stable(src), 100000);

    return true;
}

bool clk_src_disable(clk_src_t src)
{
    clk_src_info_t *clk_src_sel = &clk_src_info[src];

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
    clk_src_info_t *clk_src_sel = &clk_src_info[src];

    if (clk_src_sel->reg_ctrl == nullptr)
    {
        return false;
    }

    // Read ENABLED
    return (*clk_src_sel->reg_status & (1u << 12)) != 0;
}

bool clk_src_is_stable(clk_src_t src)
{
    clk_src_info_t *clk_src_sel = &clk_src_info[src];

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
                case CLK_SYS_SRC_ROSC: return CLK_SRC_ROSC;
                case CLK_SYS_SRC_XOSC: return CLK_SRC_XOSC;
                default: return CLK_SRC_INVALID;
            }
            break;
        case CLK_PERI:
            switch (*clk_info[CLK_PERI].reg_ctrl & CLK_PERI_SRC_MASK)
            {
                case CLK_PERI_SRC_SYS: return CLK_SRC_SYS;
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
    clk_info_t *clk_sel = &clk_info[clk];
    clk_src_t src_old = clk_get_src(clk);

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
    }

    if (glitchless)
    {
        if (!clk_set_src_internal(clk, src, clk_sel->src_mask))
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
    clk_info_t* clk_sel = &clk_info[clk];
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
                default: return false;
            }
            break;
        case CLK_PERI:
            switch (src)
            {
                case CLK_SRC_SYS: val = CLK_PERI_SRC_SYS; break;
                case CLK_SRC_ROSC: val = CLK_PERI_SRC_ROSC; break;
                case CLK_SRC_XOSC: val = CLK_PERI_SRC_XOSC; break;
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
    clk_info_t* clk_sel = &clk_info[clk];

    // Read masked SELECTED register, any set bit indicates aux uses this clock source
    return (*clk_sel->reg_sel & clk_sel->sel_mask) != 0;
}

uint32_t clk_get_freq(clk_t clk)
{
    switch (clk_get_src(clk))
    {
        case CLK_SRC_REF: return clk_get_freq(CLK_REF);
        case CLK_SRC_SYS: return clk_get_freq(CLK_SYS);
        case CLK_SRC_ROSC: return CLK_SRC_ROSC_FREQ;
        case CLK_SRC_XOSC: return CLK_SRC_XOSC_FREQ;
        case CLK_SRC_LPOSC: return CLK_SRC_ROSC_LPOSC;
        default: return 0;
    }
}