#include "clk.h"

bool clk_init()
{
    if (!clk_enable(CLK_PERI))
    {
        return false;
    }

    if (!clk_src_enable(CLK_SRC_XOSC))
    {
        return false;
    }

    if (!clk_set_src(CLK_REF, CLK_SRC_XOSC))
    {
        return false;
    }

    if (!clk_set_src(CLK_PERI, CLK_SRC_XOSC))
    {
        return false;
    }

    if (clk_get_src(CLK_SYS) != CLK_SRC_PLL_SYS)
    {
        if (!clk_pll_reset())
        {
            return false;
        }

        if (!clk_pll_enable(CLK_PLL_SYS, 1, 125, 5, 2))
        {
            return false;
        }

        if (!clk_set_src(CLK_SYS, CLK_SRC_PLL_SYS))
        {
            return false;
        }
    }

    return true;
}

size_t clk_get_info(clk_info_t *clks, const size_t len)
{
    const clk_t types[] = { CLK_REF, CLK_SYS, CLK_PERI };
    const size_t count = len < LEN(types) ? len : LEN(types);

    for (size_t i = 0; i < count; i++)
    {
        const char *clk_name;
        const char *clk_src_name;

        switch (types[i])
        {
            case CLK_REF: clk_name = "CLK_REF"; break;
            case CLK_SYS: clk_name = "CLK_SYS"; break;
            case CLK_PERI: clk_name = "CLK_PERI"; break;
            default: clk_name = "INVALID"; break;
        }

        switch (clk_get_src(types[i]))
        {
            case CLK_SRC_REF: clk_src_name = "CLK_SRC_REF"; break;
            case CLK_SRC_SYS: clk_src_name = "CLK_SRC_SYS"; break;
            case CLK_SRC_ROSC: clk_src_name = "CLK_SRC_ROSC"; break;
            case CLK_SRC_XOSC: clk_src_name = "CLK_SRC_XOSC"; break;
            case CLK_SRC_LPOSC: clk_src_name = "CLK_SRC_LPOSC"; break;
            case CLK_SRC_PLL_SYS: clk_src_name = "CLK_SRC_PLL_SYS"; break;
            case CLK_SRC_PLL_USB: clk_src_name = "CLK_SRC_PLL_USB"; break;
            default: clk_src_name = "INVALID"; break;
        }

        strncpy(clks[i].name, clk_name, sizeof(clks[i].name));
        strncpy(clks[i].src, clk_src_name, sizeof(clks[i].src));

        clks[i].enabled = clk_is_enabled(types[i]);
        clks[i].freq = clk_get_freq(types[i]);
    }

    return count;
}