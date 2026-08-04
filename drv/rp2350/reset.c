#include "reset.h"

void reset_subsys(reset_subsys_t subsys)
{
    uint32_t bit = (1 << subsys);

    // Assert subsystem reset
    *RESET_REG_RESET |= bit;

    // Wait for flag to clear
    while ((*RESET_REG_RESET_DONE & bit) != 0);

    // Deassert subsystem reset
    *RESET_REG_RESET &= ~bit;

    // Wait for reset to complete
    while ((*RESET_REG_RESET_DONE & bit) == 0);
}