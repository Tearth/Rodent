#include "cfg.h"

static void cfg_parse(const char *section, const char *name, const char *value, cfg_boot_t *cfg);

bool cfg_load(const char *path, cfg_boot_t *cfg)
{
    fs_fhandle_t handle = {};
    int32_t read_bytes = 0;
    uint32_t pos = 0;
    char buf[256];

    if (!fs_file_open(path, &handle))
    {
        return log_msg(LOG_LEVEL_FAIL, "Failed to open configuration file"), false;
    }

    do
    {
        if ((read_bytes = fs_file_read(&handle, buf, sizeof(buf))) < 0)
        {
            return log_msg(LOG_LEVEL_FAIL, "Failed to read configuration file"), false;
        }

        uint32_t line_from = 0;
        uint32_t str_from = 0;
        char name[NAME_LEN];
        char value[VALUE_LEN];
        char section[SECTION_LEN];

        for (size_t c = 0; c < (size_t)read_bytes; c++)
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
    } while (read_bytes == sizeof(buf));

    return log_fmt(LOG_LEVEL_OK, "Loaded ", path, nullptr), true;
}

static void cfg_parse(const char *section, const char *name, const char *value, cfg_boot_t *cfg)
{
    if (strcmp(section, "kernel") == 0)
    {
        if (strcmp(name, "path") == 0)
        {
            memcpy(cfg->kernel_path, value, VALUE_LEN);
        }
    }
    else if (strcmp(section, "srv") == 0)
    {
        for (size_t i = 0; i < MAX_BOOT_PROCS; i++)
        {
            char name_buf[NAME_LEN];
            char num_buf[8];

            itoa(i, num_buf, 10);

            strcpy(name_buf, "path");
            strcat(name_buf, num_buf);

            if (strcmp(name, name_buf) == 0)
            {
                memcpy(cfg->srv_path[i], value, VALUE_LEN);
            }
        }
    }
}