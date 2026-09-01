#ifndef CFG_H
#define CFG_H

#include <stddef.h>
#include "fs/fs.h"
#include "shared/defs.h"
#include "log.h"

#define NAME_LEN 64
#define VALUE_LEN 128
#define SECTION_LEN 64

typedef struct cfg_boot
{
    char kernel_path[VALUE_LEN];
    char srv_path[MAX_BOOT_PROCS][VALUE_LEN];
} cfg_boot_t;

bool cfg_load(const char *path, cfg_boot_t *cfg);

#endif