#include "cfg.h"

static void cfg_parse(const char *section, const char *name, const char *value, cfg_boot_t *cfg);

bool cfg_load(const char *path, cfg_boot_t *cfg)
{
    fs_fhandle_t handle = {};
    uint32_t pos = 0;
    char buf[256] = {};

    if (!fs_file_open(path, &handle))
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to open configuration file"), false;
    }

    do
    {
        int32_t read_bytes = fs_file_read(&handle, buf, sizeof(buf));

        if (read_bytes < 0)
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to read configuration file"), false;
        }

        uint32_t line_from = 0;
        uint32_t str_from = 0;
        char name[NAME_LEN];
        char value[VALUE_LEN];
        char section[SECTION_LEN];

        for (size_t c = 0; c < read_bytes; c++)
        {
            if (buf[c] == '[')
            {
                str_from = c + 1;
            }
            else if (buf[c] == ']')
            {
                memcpy(section, buf + str_from, c - str_from);
                section[c - str_from] = 0;
                str_from = c + 1;
            }
            else if (buf[c] == '=')
            {
                memcpy(name, buf + str_from, c - str_from);
                name[c - str_from] = 0;
                str_from = c + 1;
            }
            if (buf[c] == '\n')
            {
                if (c != str_from)
                {
                    memcpy(value, buf + str_from, c - str_from);
                    value[c - str_from] = 0;

                    cfg_parse(section, name, value, cfg);
                }

                line_from = c + 1;
                str_from = c + 1;
            }
        }

        if (read_bytes == sizeof(buf))
        {
            pos += line_from;

            if (!fs_file_seek(&handle, pos))
            {
                return log_msg(LOG_LEVEL_FAIL, "Failed to seek kernel ELF"), false;
            }
        }
        else
        {
            break;
        }
    } while (true);

    return true;
}

static void cfg_parse(const char *section, const char *name, const char *value, cfg_boot_t *cfg)
{
    if (strcmp(section, "kernel") == 0)
    {
        if (strcmp(name, "path") == 0) memcpy(cfg->kernel_path, value, VALUE_LEN);
    }
    else if (strcmp(section, "srv") == 0)
    {
        if (strcmp(name, "path0") == 0) memcpy(cfg->srv_path[0], value, VALUE_LEN);
        if (strcmp(name, "path1") == 0) memcpy(cfg->srv_path[1], value, VALUE_LEN);
        if (strcmp(name, "path2") == 0) memcpy(cfg->srv_path[2], value, VALUE_LEN);
        if (strcmp(name, "path3") == 0) memcpy(cfg->srv_path[3], value, VALUE_LEN);
        if (strcmp(name, "path4") == 0) memcpy(cfg->srv_path[4], value, VALUE_LEN);
        if (strcmp(name, "path5") == 0) memcpy(cfg->srv_path[5], value, VALUE_LEN);
        if (strcmp(name, "path6") == 0) memcpy(cfg->srv_path[6], value, VALUE_LEN);
        if (strcmp(name, "path7") == 0) memcpy(cfg->srv_path[7], value, VALUE_LEN);
    }
}