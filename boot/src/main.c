#include <stdlib.h>
#include "arch/arch.h"
#include "cfg/config.h"
#include "cpu/cpu.h"
#include "fs/fs.h"
#include "elf.h"
#include "log.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static bool init_hw();
static bool init_fs();
static bool init_kernel();
static void halt();

int main()
{
    if (!init_hw())
    {
        halt();
    }

    if (!init_fs())
    {
        halt();
    }

    if (!init_kernel())
    {
        halt();
    }

    halt();
}

bool init_hw()
{
    if (!clk_init())
    {
        return false;
    }

    if (!uart_init())
    {
        return false;
    }

    log_msg(LOG_LEVEL_OK, "Rodent Bootloader");
    log_msg(LOG_LEVEL_OK, "Started clocks");

    clk_info_t clks[8];
    uart_info_t uarts[8];

    for (int i = 0; i < clk_get_list(clks, 8); i++)
    {
        char freq_buf[16];
        char *enabled_buf;

        itoa(clks[i].freq / 1'000'000, freq_buf, 10);

        switch (clks[i].enabled)
        {
            case true: enabled_buf = "active"; break;
            case false: enabled_buf = "inactive"; break;
        }

        log_fmt(LOG_LEVEL_INFO, " ", clks[i].name, " @ ", clks[i].src, " (", freq_buf, " MHz), ", enabled_buf, nullptr);
    }

    log_msg(LOG_LEVEL_OK, "Started UART");

    for (int i = 0; i < uart_get_list(uarts, 8); i++)
    {
        char baudrate_buf[16];
        char data_bits_buf[16];
        char stop_bits_buf[16];
        char *enabled_buf;

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

bool init_fs()
{
    if (fs_mount((void*)FS_BASE_ADDR))
    {
        fs_info_t info = {};
        char base_addr_from_buf[16];
        char base_addr_to_buf[16];
        char size_buf[16];

        fs_get_info(&info);
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

bool init_kernel()
{
    fs_fhandle_t handle = {};
    elf_header_t elf_header = {};
    uint8_t buf[64];

    if (!fs_file_open(KERNEL_FILENAME, &handle))
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to open kernel ELF"), false;
    }

    fs_file_read(&handle, buf, sizeof(buf));
    memcpy(&elf_header, buf, sizeof(elf_header_t));

    if (elf_header.signature != 0x464c457f)
    {
        return log_msg(LOG_LEVEL_FAIL, "Invalid ELF signature"), false;
    }

    log_msg(LOG_LEVEL_OK, "Found valid kernel executable");

    uint32_t data_from = UINT32_MAX;
    uint32_t data_to = 0;
    uint32_t bss_from = UINT32_MAX;
    uint32_t bss_to = 0;

    for (int i = 0; i < elf_header.phnum; i++)
    {
        elf_pheader_t pheader = {};

        fs_file_seek(&handle, elf_header.phoff + sizeof(elf_pheader_t) * i);
        fs_file_read(&handle, buf, sizeof(buf));
        memcpy(&pheader, buf, sizeof(elf_pheader_t));

        if (pheader.type == 0x1)
        {
            int32_t data_left = pheader.fsize;
            uint8_t *vaddr_ptr = (uint8_t*)pheader.vaddr;

            fs_file_seek(&handle, pheader.offset);

            do
            {
                fs_file_read(&handle, buf, sizeof(buf));
                memcpy(vaddr_ptr, buf, MIN(data_left, sizeof(buf)));

                vaddr_ptr += sizeof(buf);
                data_left -= sizeof(buf);
            }
            while (data_left > 0);

            data_from = MIN(data_from, pheader.vaddr);
            data_to = MAX(data_to, pheader.vaddr + pheader.fsize);

            memset((uint8_t*)pheader.vaddr + pheader.fsize, 0, pheader.msize - pheader.fsize);
            bss_from = MIN(bss_from, pheader.vaddr + pheader.fsize);
            bss_to = MAX(bss_to, pheader.vaddr + pheader.msize);
        }
    }

    char entry_buf[16];
    char addr_from_buf[16];
    char addr_to_buf[16];
    char addr_size_buf[16];

    itoa(elf_header.entry, entry_buf, 16);
    itoa(data_from, addr_from_buf, 16);
    itoa(data_to, addr_to_buf, 16);
    itoa(data_to - data_from, addr_size_buf, 10);

    log_fmt(LOG_LEVEL_OK, "Loaded ", KERNEL_FILENAME, " into memory", nullptr);
    log_fmt(LOG_LEVEL_INFO, " Data @ 0x", addr_from_buf, "-0x", addr_to_buf, " (", addr_size_buf, " B)", nullptr);

    itoa(bss_from, addr_from_buf, 16);
    itoa(bss_to, addr_to_buf, 16);
    itoa(bss_to - bss_from, addr_size_buf, 10);

    log_fmt(LOG_LEVEL_INFO, " BSS @ 0x", addr_from_buf, "-0x", addr_to_buf, " (", addr_size_buf, " B)", nullptr);
    log_fmt(LOG_LEVEL_INFO, "Jumping to kernel @ 0x", entry_buf, nullptr);
    log_msg(LOG_LEVEL_INFO, "------------------------------");

    jmp((void*)elf_header.entry);
}

void halt()
{
    while(1);
}