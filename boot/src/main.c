#include <stdlib.h>
#include <hal.h>
#include "arch/arch.h"
#include "cfg/config.h"
#include "fs/fs.h"
#include "log.h"
#include "elf.h"

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
    if (!hal_clk_init())
    {
        return false;
    }

    if (!hal_uart_init())
    {
        return false;
    }

    log_msg(LOG_LEVEL_OK, "PicOS Bootloader");
    log_msg(LOG_LEVEL_OK, "Started clocks");

    hal_clk_info_t clks[8];
    hal_uart_info_t uarts[8];

    for (int i = 0; i < hal_clk_get_list(clks, 8); i++)
    {
        char freq_buf[16];
        char *enabled_buff;

        itoa(clks[i].freq / 1'000'000, freq_buf, 10);

        switch (clks[i].enabled)
        {
            case true: enabled_buff = "active"; break;
            case false: enabled_buff = "inactive"; break;
        }

        log_fmt(LOG_LEVEL_INFO, " ", clks[i].name, " @ ", clks[i].src, " (", freq_buf, " MHz), ", enabled_buff, nullptr);
    }

    log_msg(LOG_LEVEL_OK, "Started UART");

    for (int i = 0; i < hal_uart_get_list(uarts, 8); i++)
    {
        char baudrate_buf[16];
        char data_bits_buf[16];
        char stop_bits_buf[16];
        char *enabled_buff;

        itoa(uarts[i].baudrate, baudrate_buf, 10);
        itoa(uarts[i].data_bits, data_bits_buf, 10);
        itoa(uarts[i].stop_bits, stop_bits_buf, 10);

        switch (uarts[i].enabled)
        {
            case true: enabled_buff = "active"; break;
            case false: enabled_buff = "inactive"; break;
        }

        log_fmt(LOG_LEVEL_INFO, " ", uarts[i].name, " @ ", baudrate_buf, "/", data_bits_buf, "/", stop_bits_buf, ", ", enabled_buff, nullptr);
    }

    return true;
}

bool init_fs()
{
    if (fs_mount((void*)FS_BASE_ADDR))
    {
        fs_info_t info;
        fs_get_info(&info);

        return log_fmt(LOG_LEVEL_OK, "Mounted filesystem ", info.name), true;
    }
    else
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to mount filesystem"), false;
    }
}

bool init_kernel()
{
    fs_fhandle_t handle;
    elf_header_t elf_header;
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
        elf_pheader_t pheader;

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

            uint32_t bss_length = pheader.msize - pheader.fsize;

            memset((uint8_t*)pheader.vaddr + pheader.fsize, 0, bss_length);
            bss_from = MIN(bss_from, pheader.vaddr + pheader.fsize);
            bss_to = MAX(bss_to, pheader.vaddr + pheader.msize);
        }
    }

    char entry_buf[16];
    char addr_from[16];
    char addr_to[16];
    char addr_size[16];

    itoa(elf_header.entry, entry_buf, 16);
    itoa(data_from, addr_from, 16);
    itoa(data_to, addr_to, 16);
    itoa(data_to - data_from, addr_size, 10);

    log_fmt(LOG_LEVEL_OK, "Loaded ", KERNEL_FILENAME, " into memory", nullptr);
    log_fmt(LOG_LEVEL_INFO, " Data @ 0x", addr_from, "-0x", addr_to, " (", addr_size, " B)", nullptr);

    itoa(bss_from, addr_from, 16);
    itoa(bss_to, addr_to, 16);
    itoa(bss_to - bss_from, addr_size, 10);

    log_fmt(LOG_LEVEL_INFO, " BSS @ 0x", addr_from, "-0x", addr_to, " (", addr_size, " B)", nullptr);
    log_fmt(LOG_LEVEL_INFO, "Jumping to kernel @ 0x", entry_buf, nullptr);
    log_msg(LOG_LEVEL_INFO, "------------------------------");

    jmp((void*)elf_header.entry);
}

void halt()
{
    while(1);
}