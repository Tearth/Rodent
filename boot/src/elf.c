#include "elf.h"

bool elf_load(const char *path, elf_data_t *data)
{
    fs_fhandle_t handle = {};
    elf_header_t elf_header = {};
    uint8_t buf[64];

    if (!fs_file_open(path, &handle))
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to open ELF file"), false;
    }

    if (fs_file_read(&handle, buf, sizeof(buf)) < 0)
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to read ELF file"), false;
    }

    memcpy(&elf_header, buf, sizeof(elf_header_t));

    if (elf_header.signature != ELF_MAGIC)
    {
        return log_msg(LOG_LEVEL_FAIL, "Invalid ELF signature"), false;
    }

    if (elf_header.type != ELF_TYPE_EXEC)
    {
        return log_msg(LOG_LEVEL_FAIL, "Invalid ELF type"), false;
    }

    if (elf_header.machine != ELF_MACHINE)
    {
        return log_msg(LOG_LEVEL_FAIL, "Invalid ELF machine"), false;
    }

    uint32_t data_from = UINT32_MAX;
    uint32_t data_to = 0;
    uint32_t bss_from = UINT32_MAX;
    uint32_t bss_to = 0;

    for (size_t i = 0; i < elf_header.phnum; i++)
    {
        elf_pheader_t pheader = {};

        if (!fs_file_seek(&handle, elf_header.phoff + sizeof(elf_pheader_t) * i))
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to seek ELF file"), false;
        }

        if (fs_file_read(&handle, buf, sizeof(buf)) < 0)
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to read ELF file"), false;
        }

        memcpy(&pheader, buf, sizeof(elf_pheader_t));

        if (pheader.type == 0x1)
        {
            int32_t data_left = pheader.fsize;
            uint8_t *vaddr_ptr = (uint8_t*)pheader.vaddr;

            if (!fs_file_seek(&handle, pheader.offset))
            {
                return log_msg(LOG_LEVEL_FAIL, "Failed to seek ELF file"), false;
            }

            do
            {
                if (fs_file_read(&handle, buf, sizeof(buf)) < 0)
                {
                    return log_msg(LOG_LEVEL_FAIL, "Failed to read ELF file"), false;
                }

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

    log_fmt(LOG_LEVEL_OK, "Loaded ", path, nullptr);
    log_fmt(LOG_LEVEL_INFO, " Data @ 0x", addr_from_buf, "-0x", addr_to_buf, " (", addr_size_buf, " B)", nullptr);

    itoa(bss_from, addr_from_buf, 16);
    itoa(bss_to, addr_to_buf, 16);
    itoa(bss_to - bss_from, addr_size_buf, 10);

    log_fmt(LOG_LEVEL_INFO, " BSS @ 0x", addr_from_buf, "-0x", addr_to_buf, " (", addr_size_buf, " B)", nullptr);
    log_fmt(LOG_LEVEL_INFO, " Entry point @ 0x", entry_buf, nullptr);

    data->base = (void*)data_from;
    data->entry = (void*)elf_header.entry;
    data->size = bss_to - data_from;

    return true;
}