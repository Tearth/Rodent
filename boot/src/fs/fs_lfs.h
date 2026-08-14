#ifndef FS_LFS_H
#define FS_LFS_H

#define FS_CACHE_SIZE 16

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <lfs.h>

typedef struct fs_info
{
    char name[64];
} fs_info_t;

typedef struct fs_fhandle
{
    lfs_file_t file;
    struct lfs_file_config cfg;
    uint8_t buf[16];
} fs_fhandle_t;

bool fs_mount(void* base);
bool fs_file_open(const char *path, fs_fhandle_t *handle);
void fs_file_read(fs_fhandle_t *handle, uint8_t *buf, uint32_t size);
void fs_file_seek(fs_fhandle_t *handle, uint32_t pos);
void fs_get_info(fs_info_t *info);

int lfs_read(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, void *buf, lfs_size_t size);
int lfs_prog(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, const void *buf, lfs_size_t size);
int lfs_erase(const struct lfs_config *cfg, lfs_block_t block);
int lfs_sync(const struct lfs_config *cfg);

#endif