#ifndef FS_LFS_H
#define FS_LFS_H

#define FS_LFS_MAGIC
#define FS_LFS_CACHE_SIZE 16
#define FS_LFS_BLOCK_SIZE 4096
#define FS_LFS_BLOCK_CYCLES 512
#define FS_LFS_MAGIC_WORDH 0x7474696c
#define FS_LFS_MAGIC_WORDL 0x7366656c

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <lfs.h>
#include "mcu/mcu.h"

typedef struct lfs_config lfs_config_t;
typedef struct lfs_file_config lfs_file_config_t;

typedef struct fs_info
{
    char name[16];
    uint32_t *base_addr;
    uint32_t size;
} fs_info_t;

typedef struct fs_fhandle
{
    lfs_file_t file;
    lfs_file_config_t cfg;
    uint8_t buf[16];
} fs_fhandle_t;

bool fs_mount(void* base);
bool fs_file_open(const char *path, fs_fhandle_t *handle);
int32_t fs_file_read(fs_fhandle_t *handle, void *buf, uint32_t size);
uint32_t fs_file_pos(fs_fhandle_t *handle);
bool fs_file_seek(fs_fhandle_t *handle, uint32_t pos);
bool fs_get_info(fs_info_t *info);

int lfs_read(const lfs_config_t *cfg, lfs_block_t block, lfs_off_t off, void *buf, lfs_size_t size);
int lfs_prog(const lfs_config_t *cfg, lfs_block_t block, lfs_off_t off, const void *buf, lfs_size_t size);
int lfs_erase(const lfs_config_t *cfg, lfs_block_t block);
int lfs_sync(const lfs_config_t *cfg);

#endif