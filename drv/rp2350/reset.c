#include "reset.h"

void reset_subsys(reset_subsys_t subsys)
{
    uint32_t bit = (1 << subsys);

    // Reset the subsystem
    *RESET_REG_RESET |= bit;

    // Wait for reset to complete
    while ((*RESET_REG_RESET_DONE & bit) == 0);

    // Unreset the subsystem
    *RESET_REG_RESET &= ~bit;

    // Wait for unreset to complete
    while ((*RESET_REG_RESET_DONE & bit) != 0);
}