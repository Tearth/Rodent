#include "fs_lfs.h"

lfs_t lfs;
uint8_t read_buf[FS_CACHE_SIZE];
uint8_t prog_buf[FS_CACHE_SIZE];
uint8_t lookahead_buf[FS_CACHE_SIZE];

struct lfs_config cfg = {
    .read  = lfs_read,
    .prog  = lfs_prog,
    .erase = lfs_erase,
    .sync  = lfs_sync,

    .read_size = FS_CACHE_SIZE,
    .prog_size = FS_CACHE_SIZE,
    .cache_size = FS_CACHE_SIZE,
    .lookahead_size = FS_CACHE_SIZE,
    .block_size = 4096,
    .block_cycles = 500,

    .read_buffer = read_buf,
    .prog_buffer = prog_buf,
    .lookahead_buffer = lookahead_buf
};

bool fs_mount(void* base_addr)
{
    if (base_addr == NULL)
    {
        return false;
    }

    uint32_t v1 = *((uint32_t*)base_addr + 0);
    uint32_t v2 = *((uint32_t*)base_addr + 1);
    uint32_t v3 = *((uint32_t*)base_addr + 2);
    uint32_t v4 = *((uint32_t*)base_addr + 3);

    // Check littlefs signature
    if (v1 == 1 && v2 == 4160688112 && v3 == 1953786220 && v4 == 1936090476)
    {
        cfg.context = base_addr;
    }
    else
    {
        return false;
    }

    return lfs_mount(&lfs, &cfg) == LFS_ERR_OK;
}

bool fs_file_open(const char *path, fs_fhandle_t *handle)
{
    if (handle->cfg.buffer != handle->buf)
    {
        handle->cfg.buffer = handle->buf;
    }

    return lfs_file_opencfg(&lfs, &handle->file, path, LFS_O_RDONLY, &handle->cfg) == LFS_ERR_OK;
}

void fs_file_read(fs_fhandle_t *handle, uint8_t *buf, uint32_t size)
{
    lfs_file_read(&lfs, &handle->file, buf, size);
}

void fs_file_seek(fs_fhandle_t *handle, uint32_t pos)
{
    lfs_file_seek(&lfs, &handle->file, pos, LFS_SEEK_SET);
}

void fs_get_info(fs_info_t *info)
{
    strncpy(info->name, "LittleFS", sizeof(info->name));
}

int lfs_read(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, void *buf, lfs_size_t size)
{
    return memcpy(buf, (const void*)((uint8_t*)cfg->context + block * cfg->block_size + off), size), LFS_ERR_OK;
}

int lfs_prog(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, const void *buf, lfs_size_t size)
{
    return LFS_ERR_OK;
}

int lfs_erase(const struct lfs_config *cfg, lfs_block_t block)
{
    return LFS_ERR_OK;
}

int lfs_sync(const struct lfs_config *cfg)
{
    return LFS_ERR_OK;
}