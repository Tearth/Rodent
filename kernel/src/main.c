#include <stdlib.h>
#include <hal.h>
#include "log.h"

static bool init_systime();
static void halt();

int main()
{
    log_msg(LOG_LEVEL_OK, "Rodent Kernel");

    if (!init_systime())
    {
        halt();
    }

    while (1);
}

bool init_systime()
{
    char buf[16];

    hal_systime_init();
    utoa(hal_systime_get_current(), buf, 10);

    log_msg(LOG_LEVEL_OK, "Started system time");
    log_fmt(LOG_LEVEL_INFO, " Current value @ ", buf, nullptr);

    return true;
}

void halt()
{
    while(1);
}