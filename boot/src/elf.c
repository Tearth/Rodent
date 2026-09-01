#include "elf.h"

static bool elf_get_base_vaddr(fs_fhandle_t *handle, elf_header_t *elf_header, uint32_t *base_vaddr);
static bool elf_get_strtab_offset(fs_fhandle_t *handle, elf_header_t *elf_header, uint32_t *offset);
static bool elf_get_strtab_entry(fs_fhandle_t *handle, uint32_t strtab_offset, char *val, size_t len);

bool elf_load(const char *path, elf_data_t *data, void *addr)
{
    char buf[256];
    fs_fhandle_t handle;
    elf_header_t elf_header;

    if (!fs_file_open(path, &handle))
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to open ELF file"), false;
    }

    if (fs_file_read(&handle, &elf_header, sizeof(elf_header)) < 0)
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to read ELF file"), false;
    }

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

    uint32_t base_vaddr;
    uint32_t strtab_offset;
    int32_t offset = 0;

    if (!elf_get_base_vaddr(&handle, &elf_header, &base_vaddr))
    {
        return log_msg(LOG_LEVEL_FAIL, "Invalid base vaddr"), false;
    }

    if (!elf_get_strtab_offset(&handle, &elf_header, &strtab_offset))
    {
        return log_msg(LOG_LEVEL_FAIL, "Invalid strtab offset"), false;
    }

    if (addr != nullptr)
    {
        offset = (uint32_t)addr - base_vaddr;
    }

    uint32_t data_from = UINT32_MAX;
    uint32_t data_to = 0;
    uint32_t bss_from = UINT32_MAX;
    uint32_t bss_to = 0;

    for (size_t i = 0; i < elf_header.phnum; i++)
    {
        elf_pheader_t pheader;

        if (!fs_file_seek(&handle, elf_header.phoff + sizeof(elf_pheader_t) * i))
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to seek ELF file"), false;
        }

        if (fs_file_read(&handle, &pheader, sizeof(elf_pheader_t)) < 0)
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to read ELF file"), false;
        }

        // PT_LOAD
        if (pheader.type == 0x1)
        {
            int32_t data_left = pheader.fsize;
            uint8_t *vaddr = (uint8_t*)(pheader.vaddr);

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

                memcpy(vaddr + offset, buf, MIN(data_left, sizeof(buf)));

                vaddr += sizeof(buf);
                data_left -= sizeof(buf);
            }
            while (data_left > 0);

            data_from = MIN(data_from, pheader.vaddr);
            data_to = MAX(data_to, pheader.vaddr + pheader.fsize);

            memset((uint8_t*)(pheader.vaddr + pheader.fsize + offset), 0, pheader.msize - pheader.fsize);
            bss_from = MIN(bss_from, pheader.vaddr + pheader.fsize);
            bss_to = MAX(bss_to, pheader.vaddr + pheader.msize);
        }
    }

    for (size_t i = 0; i < elf_header.shnum; i++)
    {
        char name[256];
        elf_sheader_t sheader;

        if (!fs_file_seek(&handle, elf_header.shoff + sizeof(elf_sheader_t) * i))
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to seek ELF file"), false;
        }

        if (fs_file_read(&handle, &sheader, sizeof(elf_sheader_t)) < 0)
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to read ELF file"), false;
        }

        if (!elf_get_strtab_entry(&handle, strtab_offset + sheader.name, name, sizeof(name)))
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to read strtab"), false;
        }

        if (strcmp(name, ".got") == 0 || strcmp(name, ".got.plt") == 0)
        {
            for (size_t j = 0; j < sheader.size / sheader.entsize; j++)
            {
                uint32_t *entry = (uint32_t *)((uint8_t*)sheader.addr + offset + j * sheader.entsize);

                // Some addresses like 0x0 or 0xffffffff probably shouldn't be touched
                if (*entry != 0 && *entry != UINT32_MAX)
                {
                    *entry += offset;
                }
            }
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

    data->base = (void*)(data_from + offset);
    data->entry = (void*)(elf_header.entry + offset);
    data->size = bss_to - data_from;

    return true;
}

static bool elf_get_base_vaddr(fs_fhandle_t *handle, elf_header_t *elf_header, uint32_t *base_vaddr)
{
    *base_vaddr = UINT32_MAX;

    for (size_t i = 0; i < elf_header->phnum; i++)
    {
        elf_pheader_t pheader;

        if (!fs_file_seek(handle, elf_header->phoff + sizeof(elf_pheader_t) * i))
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to seek ELF file"), false;
        }

        if (fs_file_read(handle, &pheader, sizeof(elf_pheader_t)) < 0)
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to read ELF file"), false;
        }

        // PT_LOAD
        if (pheader.type == 0x1)
        {
            *base_vaddr = MIN(*base_vaddr, pheader.vaddr);
        }
    }

    return true;
}

static bool elf_get_strtab_offset(fs_fhandle_t *handle, elf_header_t *elf_header, uint32_t *offset)
{
    elf_sheader_t sheader;
    uint32_t pos_old = fs_file_pos(handle);

    if (!fs_file_seek(handle, elf_header->shoff + sizeof(elf_sheader_t) * elf_header->shstrndx))
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to seek ELF file"), false;
    }

    if (fs_file_read(handle, &sheader, sizeof(elf_sheader_t)) < 0)
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to read ELF file"), false;
    }

    fs_file_seek(handle, pos_old);

    return *offset = sheader.offset, true;
}

static bool elf_get_strtab_entry(fs_fhandle_t *handle, uint32_t strtab_offset, char *val, size_t len)
{
    char buf[256];
    uint32_t pos_old = fs_file_pos(handle);

    if (!fs_file_seek(handle, strtab_offset))
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to seek ELF file"), false;
    }

    if (fs_file_read(handle, buf, sizeof(buf)) < 0)
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to read ELF file"), false;
    }

    strncpy(val, buf, len);
    fs_file_seek(handle, pos_old);

    return true;
}