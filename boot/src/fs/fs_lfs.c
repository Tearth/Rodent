#include "fs_lfs.h"

static lfs_t lfs;
static uint8_t read_buf[FS_LFS_CACHE_SIZE];
static uint8_t prog_buf[FS_LFS_CACHE_SIZE];
static uint8_t lookahead_buf[FS_LFS_CACHE_SIZE];

static lfs_config_t cfg = {
    .read  = lfs_read,
    .prog  = lfs_prog,
    .erase = lfs_erase,
    .sync  = lfs_sync,

    .read_size = FS_LFS_CACHE_SIZE,
    .prog_size = FS_LFS_CACHE_SIZE,
    .cache_size = FS_LFS_CACHE_SIZE,
    .lookahead_size = FS_LFS_CACHE_SIZE,
    .block_size = FS_LFS_BLOCK_SIZE,
    .block_cycles = FS_LFS_BLOCK_CYCLES,

    .read_buffer = read_buf,
    .prog_buffer = prog_buf,
    .lookahead_buffer = lookahead_buf
};

bool fs_mount(void* base_addr)
{
    uint32_t m1 = *((uint32_t*)base_addr + 2);
    uint32_t m2 = *((uint32_t*)base_addr + 3);

    if (m1 == FS_LFS_MAGIC_WORDH && m2 == FS_LFS_MAGIC_WORDL)
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

int32_t fs_file_read(fs_fhandle_t *handle, void *buf, uint32_t size)
{
    return lfs_file_read(&lfs, &handle->file, buf, size);
}

uint32_t fs_file_pos(fs_fhandle_t *handle)
{
    return lfs_file_tell(&lfs, &handle->file);
}

bool fs_file_seek(fs_fhandle_t *handle, uint32_t pos)
{
    return lfs_file_seek(&lfs, &handle->file, pos, LFS_SEEK_SET) >= 0;
}

bool fs_get_info(fs_info_t *info)
{
    struct lfs_fsinfo fsinfo = {};

    if (lfs_fs_stat(&lfs, &fsinfo) != LFS_ERR_OK)
    {
        return false;
    }

    strncpy(info->name, "LittleFS", sizeof(info->name));

    info->base_addr = lfs.cfg->context;
    info->size = fsinfo.block_size * fsinfo.block_count;

    return true;
}

int lfs_read(const lfs_config_t *cfg, lfs_block_t block, lfs_off_t off, void *buf, lfs_size_t size)
{
    return flash_read(buf, (const void*)((uint8_t*)cfg->context + block * cfg->block_size + off), size), LFS_ERR_OK;
}

int lfs_prog(const lfs_config_t *cfg, lfs_block_t block, lfs_off_t off, const void *buf, lfs_size_t size)
{
    return LFS_ERR_OK;
}

int lfs_erase(const lfs_config_t *cfg, lfs_block_t block)
{
    return LFS_ERR_OK;
}

int lfs_sync(const lfs_config_t *cfg)
{
    return LFS_ERR_OK;
}