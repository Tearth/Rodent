#include "irq.h"

extern void _irq_handler_entry();

static void irq_user_ecall_handler(irq_state_t *state);
static void irq_timer_handler(irq_state_t *state);
static void irq_exception_handler(irq_state_t *state);
static void irq_unsupported_handler(irq_state_t *state);
static void (*timer_handler)();

bool irq_enable()
{
    uintptr_t mtvec = (uintptr_t)_irq_handler_entry;

    // Handler address has to be aligned
    if ((mtvec & 0x3) != 0)
    {
        return false;
    }

    __asm__ volatile (
        "csrw mtvec, %0\n" \
        "csrs mstatus, %1\n" \
        "csrs mie, %2\n" \
    : :
    // Set BASE with DIRECT mode (single handler for all interrupts) in MTVEC
    "r"(mtvec),
    // Set MIE (Interrupt Enable) in MSTATUS
    "r"((1u << 3)),
    // Set MTIE (Timer Interrupt Enable) in MIE
    "r"(1u << 7));

    return true;
}

bool irq_disable()
{
    __asm__ volatile (
        "csrc mstatus, %0\n" \
    : :
    // Clear MIE (Interrupt Enable) in MSTATUS
    "r"((1u << 3)));

    return true;
}

bool irq_is_enabled()
{
    uint32_t mstatus;

    // Read MIE in MSTATUS
    __asm__ volatile  (
        "csrr %0, mstatus"
    : "=r"(mstatus));

    return (mstatus & (1u << 3)) != 0;
}

void irq_attach_timer_handler(void (*handler)())
{
    timer_handler = handler;
}

void irq_handler(irq_state_t *state)
{
    switch (state->mcause)
    {
        case IRQ_CAUSE_U_ECALL_EXCEPTION: irq_user_ecall_handler(state); break;
        case IRQ_CAUSE_MACHINE_TIMER_INTERRUPT: irq_timer_handler(state); break;
        default:
        {
            if ((state->mcause & (1u << 31)) == 0)
            {
                irq_exception_handler(state);
            }
            else
            {
                irq_unsupported_handler(state);
            }

            break;
        }
    }
}

static void irq_user_ecall_handler(irq_state_t *state)
{
    // TODO: syscalls
    state->mepc += 4;
}

static void irq_timer_handler(irq_state_t *state)
{
    if (timer_handler != nullptr)
    {
        timer_handler();
    }
}

static void irq_exception_handler(irq_state_t *state)
{
    const char *name;

    switch (state->mcause)
    {
        case IRQ_CAUSE_INSTR_ALIGN_EXCEPTION: name = "IRQ_CAUSE_INSTR_ALIGN_EXCEPTION"; break;
        case IRQ_CAUSE_INSTR_FAULT_EXCEPTION: name = "IRQ_CAUSE_INSTR_FAULT_EXCEPTION"; break;
        case IRQ_CAUSE_ILLEGAL_INSTR_EXCEPTION: name = "IRQ_CAUSE_ILLEGAL_INSTR_EXCEPTION"; break;
        case IRQ_CAUSE_BREAKPOINT_EXCEPTION: name = "IRQ_CAUSE_BREAKPOINT_EXCEPTION"; break;
        case IRQ_CAUSE_LOAD_ALIGN_EXCEPTION: name = "IRQ_CAUSE_LOAD_ALIGN_EXCEPTION"; break;
        case IRQ_CAUSE_LOAD_FAULT_EXCEPTION: name = "IRQ_CAUSE_LOAD_FAULT_EXCEPTION"; break;
        case IRQ_CAUSE_STORE_ALIGN_EXCEPTION: name = "IRQ_CAUSE_STORE_ALIGN_EXCEPTION"; break;
        case IRQ_CAUSE_STORE_FAULT_EXCEPTION: name = "IRQ_CAUSE_STORE_FAULT_EXCEPTION"; break;
        case IRQ_CAUSE_U_ECALL_EXCEPTION: name = "IRQ_CAUSE_U_ECALL_EXCEPTION"; break;
        case IRQ_CAUSE_S_ECALL_EXCEPTION: name = "IRQ_CAUSE_S_ECALL_EXCEPTION"; break;
        case IRQ_CAUSE_M_ECALL_EXCEPTION: name = "IRQ_CAUSE_M_ECALL_EXCEPTION"; break;
        case IRQ_CAUSE_INSTR_PAGE_FAULT_EXCEPTION: name = "IRQ_CAUSE_INSTR_PAGE_FAULT_EXCEPTION"; break;
        case IRQ_CAUSE_LOAD_PAGE_FAULT_EXCEPTION: name = "IRQ_CAUSE_LOAD_PAGE_FAULT_EXCEPTION"; break;
        case IRQ_CAUSE_STORE_PAGE_FAULT_EXCEPTION: name = "IRQ_CAUSE_STORE_PAGE_FAULT_EXCEPTION"; break;
        default: name = "IRQ_CAUSE_UNKNOWN"; break;
    }

    char pc_buf[16];
    char mtval_buf[16];
    char sp_buf[16];

    itoa(state->mepc, pc_buf, 16);
    itoa(state->mtval, mtval_buf, 16);
    itoa(state->sp, sp_buf, 16);

    log_msg(LOG_LEVEL_FAIL, "");
    log_msg(LOG_LEVEL_FAIL, "             \\\\__\\\\");
    log_msg(LOG_LEVEL_FAIL, "         ___/  -  -\\");
    log_msg(LOG_LEVEL_FAIL, "      __/         ..\\");
    log_msg(LOG_LEVEL_FAIL, "    _/         \\____/");
    log_msg(LOG_LEVEL_FAIL, "   /            |       Fatal Exception");
    log_msg(LOG_LEVEL_FAIL, "  /             |");
    log_msg(LOG_LEVEL_FAIL, " |             /");
    log_msg(LOG_LEVEL_FAIL, " |        __   \\");
    log_msg(LOG_LEVEL_FAIL, "  \\_____//  \\__\\\\");
    log_msg(LOG_LEVEL_FAIL, "---------------------------------------");
    log_fmt(LOG_LEVEL_FAIL, "Type: ", name, nullptr);
    log_fmt(LOG_LEVEL_FAIL, "PC: 0x", pc_buf, nullptr);
    log_fmt(LOG_LEVEL_FAIL, "SP: 0x", sp_buf, nullptr);
    log_fmt(LOG_LEVEL_FAIL, "MTVAL: 0x", mtval_buf, nullptr);

    while (1);
}

static void irq_unsupported_handler(irq_state_t *state)
{
    char mcause_buf[16];

    itoa(state->mcause, mcause_buf, 16);
    log_fmt(LOG_LEVEL_WARN, "Unknown interrupt type (", mcause_buf, ")", nullptr);
}