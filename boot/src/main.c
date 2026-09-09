#include <stdlib.h>
#include "arch/arch.h"
#include "cfg/defs.h"
#include "cfg/cfg.h"
#include "shared/boot.h"
#include "shared/halt.h"
#include "mcu/mcu.h"
#include "fs/fs.h"
#include "elf.h"
#include "log.h"

static bool init_kernel(cfg_boot_t *cfg, elf_data_t *data);
static bool init_srv(cfg_boot_t *cfg, elf_data_t *kernel_data, boot_args_t *args);

int main()
{
    cfg_boot_t cfg;

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

    if (!mcu_init())
    {
        HALT();
    }

    log_msg(LOG_LEVEL_OK, "Finished CPU initialization");
    log_set_mode(LOG_MODE_UART);

    if (!fs_init())
    {
        HALT();
    }

    if (!cfg_load(BOOT_CFG, &cfg))
    {
        return log_fmt(LOG_LEVEL_FAIL, "Failed to load ", BOOT_CFG, nullptr), false;
    }

    log_fmt(LOG_LEVEL_OK, "Loaded ", BOOT_CFG, nullptr);

    boot_iface_t iface;
    iface.log_msg = log_msg;
    iface.log_vargs = log_vargs;

    boot_args_t args;
    elf_data_t kernel_data;

    if (!init_kernel(&cfg, &kernel_data))
    {
        HALT();
    }

    if (!init_srv(&cfg, &kernel_data, &args))
    {
        HALT();
    }

    log_msg(LOG_LEVEL_INFO, "Jumping to kernel");
    log_msg(LOG_LEVEL_INFO, "---------------------------------------");
    jmp(kernel_data.entry, &iface, &args);
}

static bool init_kernel(cfg_boot_t *cfg, elf_data_t *data)
{
    if (!elf_load(cfg->kernel_path, data, nullptr))
    {
        return log_fmt(LOG_LEVEL_FAIL, "Failed to load ", cfg->kernel_path, nullptr), false;
    }

    return true;
}

static bool init_srv(cfg_boot_t *cfg, elf_data_t *kernel_data, boot_args_t *args)
{
    uint32_t addr = (uint32_t)kernel_data->base + kernel_data->size;

    for (size_t i = 0; i < MAX_BOOT_PROCS; i++)
    {
        if (cfg->srv_path[i][0] != 0)
        {
            elf_data_t data;

            if (!elf_load(cfg->srv_path[i], &data, (void*)addr))
            {
                return log_fmt(LOG_LEVEL_FAIL, "Failed to load ", cfg->kernel_path, nullptr), false;
            }

            memcpy(args->procs[i].path, cfg->srv_path[i], VALUE_LEN);

            args->procs[i].type = BOOT_PROC_TYPE_SRV;
            args->procs[i].base = data.base;
            args->procs[i].entry = data.entry;
            args->procs[i].size = data.size;

            addr += data.size;
        }
    }

    return true;
}