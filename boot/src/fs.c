#include "fs.h"

bool fs_init()
{
    if (fs_mount((void *)FS_BASE_ADDR))
    {
        fs_info_t info = {};

        if (!fs_get_info(&info))
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to read filesystem info"), false;
        }

        char base_addr_from_buf[16];
        char base_addr_to_buf[16];
        char size_buf[16];

        itoa((uint32_t)info.base_addr, base_addr_from_buf, 16);
        itoa((uint32_t)(info.base_addr + info.size), base_addr_to_buf, 16);
        itoa(info.size / 1024, size_buf, 10);

        log_msg(LOG_LEVEL_OK, "Mounted filesystem");
        log_fmt(LOG_LEVEL_INFO, " ", info.name, " @ 0x", base_addr_from_buf, "-0x", base_addr_to_buf, " (", size_buf, " KB)", EOL);

        return true;
    }
    else
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to mount filesystem"), false;
    }
}