#ifndef BOOT_FS_H
#define BOOT_FS_H

#include <stdlib.h>
#include "cfg/defs.h"
#include "log.h"

#ifdef FS_LFS
#include "fs_lfs.h"
#endif

bool fs_init();

#endif