#ifndef CFG_H
#define CFG_H

#include <stddef.h>
#include "fs/fs.h"
#include "log.h"

#define NAME_LEN 64
#define VALUE_LEN 128
#define SECTION_LEN 64

typedef struct cfg_data
{
    char kernel_path[VALUE_LEN];
} cfg_data_t;

bool cfg_load(const char *path, cfg_data_t *data);

#endif