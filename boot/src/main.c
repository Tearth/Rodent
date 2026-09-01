#include <stdlib.h>
#include "arch/arch.h"
#include "cfg/defs.h"
#include "cfg/cfg.h"
#include "shared/boot.h"
#include "mcu/mcu.h"
#include "fs/fs.h"
#include "elf.h"
#include "log.h"

static bool init_hw();
static bool init_fs();
static bool init_cfg(cfg_data_t *cfg);
static bool init_kernel(cfg_data_t *cfg);
static void halt();

int main()
{
    cfg_data_t cfg;

    if (!init_hw())
    {
        halt();
    }

    if (!init_fs())
    {
        halt();
    }

    if (!init_cfg(&cfg))
    {
        halt();
    }

    if (!init_kernel(&cfg))
    {
        halt();
    }

    halt();
}

static bool init_hw()
{
    if (!clk_init())
    {
        return false;
    }

    if (!uart_init())
    {
        return false;
    }

    log_msg(LOG_LEVEL_INFO, "");
    log_msg(LOG_LEVEL_INFO, "             \\\\__\\\\");
    log_msg(LOG_LEVEL_INFO, "         ___/  -  -\\");
    log_msg(LOG_LEVEL_INFO, "      __/         ..\\");
    log_msg(LOG_LEVEL_INFO, "    _/         \\____/");
    log_msg(LOG_LEVEL_INFO, "   /            |                Rodent");
    log_msg(LOG_LEVEL_INFO, "  /             |");
    log_msg(LOG_LEVEL_INFO, " |             /");
    log_msg(LOG_LEVEL_INFO, " |        __   \\");
    log_msg(LOG_LEVEL_INFO, "  \\_____//  \\__\\\\");
    log_msg(LOG_LEVEL_INFO, "---------------------------------------");

    log_msg(LOG_LEVEL_OK, "Rodent Bootloader");
    log_msg(LOG_LEVEL_OK, "Started clocks");

    clk_info_t clks[8];
    uart_info_t uarts[8];

    for (size_t i = 0; i < clk_get_list(clks, 8); i++)
    {
        char freq_buf[16];
        const char *enabled_buf;

        itoa(clks[i].freq / 1'000'000, freq_buf, 10);

        switch (clks[i].enabled)
        {
            case true: enabled_buf = "active"; break;
            case false: enabled_buf = "inactive"; break;
        }

        log_fmt(LOG_LEVEL_INFO, " ", clks[i].name, " @ ", clks[i].src, " (", freq_buf, " MHz), ", enabled_buf, nullptr);
    }

    log_msg(LOG_LEVEL_OK, "Started UART");

    for (size_t i = 0; i < uart_get_list(uarts, 8); i++)
    {
        char baudrate_buf[16];
        char data_bits_buf[16];
        char stop_bits_buf[16];
        const char *enabled_buf;

        itoa(uarts[i].baudrate, baudrate_buf, 10);
        itoa(uarts[i].data_bits, data_bits_buf, 10);
        itoa(uarts[i].stop_bits, stop_bits_buf, 10);

        switch (uarts[i].enabled)
        {
            case true: enabled_buf = "active"; break;
            case false: enabled_buf = "inactive"; break;
        }

        log_fmt(LOG_LEVEL_INFO, " ", uarts[i].name, " @ ", baudrate_buf, "/", data_bits_buf, "/", stop_bits_buf, ", ", enabled_buf, nullptr);
    }

    return true;
}

static bool init_fs()
{
    if (fs_mount((void*)FS_BASE_ADDR))
    {
        fs_info_t info = {};
        char base_addr_from_buf[16];
        char base_addr_to_buf[16];
        char size_buf[16];

        if (!fs_get_info(&info))
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to read filesystem info"), false;
        }

        itoa((uint32_t)info.base_addr, base_addr_from_buf, 16);
        itoa((uint32_t)(info.base_addr + info.size), base_addr_to_buf, 16);
        itoa(info.size / 1024, size_buf, 10);

        log_msg(LOG_LEVEL_OK, "Mounted filesystem");
        log_fmt(LOG_LEVEL_INFO, " ", info.name, " @ 0x", base_addr_from_buf, "-0x", base_addr_to_buf, " (", size_buf, " KB)", nullptr);

        return true;
    }
    else
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to mount filesystem"), false;
    }
}

static bool init_cfg(cfg_data_t *cfg)
{
    if (!cfg_load(BOOT_CFG, cfg))
    {
        return log_fmt(LOG_LEVEL_FAIL, "Failed to load ", BOOT_CFG, nullptr), false;
    }

    return log_fmt(LOG_LEVEL_OK, "Loaded ", BOOT_CFG, nullptr), true;
}

static bool init_kernel(cfg_data_t *cfg)
{
    elf_data_t kernel;

    if (!elf_load(cfg->kernel_path, &kernel, nullptr))
    {
        return log_fmt(LOG_LEVEL_FAIL, "Failed to load ", cfg->kernel_path, nullptr), false;
    }

    log_msg(LOG_LEVEL_INFO, "Jumping to kernel");
    log_msg(LOG_LEVEL_INFO, "---------------------------------------");

    boot_iface_t iface;
    iface.log_msg = log_msg;
    iface.log_vargs = log_vargs;

    boot_args_t args;

    jmp(kernel.entry, &iface, &args);
}

static void halt()
{
    while (1);
}