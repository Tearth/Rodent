#include "fs_lfs.h"

lfs_t lfs;
uint8_t read_buffer[FS_CACHE_SIZE];
uint8_t prog_buffer[FS_CACHE_SIZE];
uint8_t lookahead_buffer[FS_CACHE_SIZE];

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

    .read_buffer = read_buffer,
    .prog_buffer = prog_buffer,
    .lookahead_buffer = lookahead_buffer
};

bool fs_init(void* base_addr)
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

bool fs_file_open(const char* path, fs_fhandle_t *handle)
{
    if (handle->config.buffer != handle->buffer)
    {
        handle->config.buffer = handle->buffer;
    }

    return lfs_file_opencfg(&lfs, &handle->file, path, LFS_O_RDONLY, &handle->config) == LFS_ERR_OK;
}

int lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    return memcpy(buffer, (const void*)((uint8_t*)c->context + block * c->block_size + off), size), LFS_ERR_OK;
}

int lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    return LFS_ERR_OK;
}

int lfs_erase(const struct lfs_config *c, lfs_block_t block)
{
    return LFS_ERR_OK;
}

int lfs_sync(const struct lfs_config *c)
{
    return LFS_ERR_OK;
}