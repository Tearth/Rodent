#include "cfg.h"

static void cfg_parse(const char *section, const char *name, const char *value, cfg_data_t *data);

bool cfg_load(const char *path, cfg_data_t *data)
{
    fs_fhandle_t handle = {};
    uint8_t buf[256] = {};
    uint32_t pos = 0;

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

                    cfg_parse(section, name, value, data);
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

static void cfg_parse(const char *section, const char *name, const char *value, cfg_data_t *data)
{
    if (strcmp(section, "kernel") == 0)
    {
        if (strcmp(name, "path") == 0)
        {
            memcpy(data->kernel_path, value, VALUE_LEN);
        }
    }
}