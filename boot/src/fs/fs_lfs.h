#ifndef FS_LFS_H
#define FS_LFS_H

#define FS_CACHE_SIZE 16

#include <stdint.h>
#include <stdbool.h>
#include <lfs.h>

typedef struct fs_fhandle
{
    lfs_file_t file;
    struct lfs_file_config config;
    uint8_t buffer[16];
} fs_fhandle_t;

bool fs_init(void* base);
bool fs_file_open(const char *path, fs_fhandle_t *handle);

int lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int lfs_erase(const struct lfs_config *c, lfs_block_t block);
int lfs_sync(const struct lfs_config *c);

#endif