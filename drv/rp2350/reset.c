#include "reset.h"

bool reset_subsys(reset_subsys_t subsys)
{
    uint32_t bit = (1u << subsys);

    // Assert subsystem reset
    *RESET_REG_RESET |= bit;

    // Wait for subsystem flag to clear
    WAIT((*RESET_REG_RESET_DONE & bit) != 0, 100);

    // Deassert subsystem reset
    *RESET_REG_RESET &= ~bit;

    // Wait for subsystem reset to complete, this might take a bit of time so timeout is higher than usual
    WAIT ((*RESET_REG_RESET_DONE & bit) == 0, 100000);

    return true;
}