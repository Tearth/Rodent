#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include <stdlib.h>
#include "regs.h"
#include "log.h"
#include "uspace.h"

typedef enum irq_cause
{
    IRQ_CAUSE_INSTR_ALIGN_EXCEPTION = 0,
    IRQ_CAUSE_INSTR_FAULT_EXCEPTION = 1,
    IRQ_CAUSE_ILLEGAL_INSTR_EXCEPTION = 2,
    IRQ_CAUSE_BREAKPOINT_EXCEPTION = 3,
    IRQ_CAUSE_LOAD_ALIGN_EXCEPTION = 4,
    IRQ_CAUSE_LOAD_FAULT_EXCEPTION = 5,
    IRQ_CAUSE_STORE_ALIGN_EXCEPTION = 6,
    IRQ_CAUSE_STORE_FAULT_EXCEPTION = 7,
    IRQ_CAUSE_U_ECALL_EXCEPTION = 8,
    IRQ_CAUSE_S_ECALL_EXCEPTION = 9,
    IRQ_CAUSE_M_ECALL_EXCEPTION = 11,
    IRQ_CAUSE_INSTR_PAGE_FAULT_EXCEPTION = 12,
    IRQ_CAUSE_LOAD_PAGE_FAULT_EXCEPTION = 13,
    IRQ_CAUSE_STORE_PAGE_FAULT_EXCEPTION = 15,

    IRQ_CAUSE_USER_SOFTWARE_INTERRUPT = 0 | (1u << 31),
    IRQ_CAUSE_SUPERVISOR_SOFTWARE_INTERRUPT = 1 | (1u << 31),
    IRQ_CAUSE_MACHINE_SOFTWARE_INTERRUPT = 3 | (1u << 31),
    IRQ_CAUSE_USER_TIMER_INTERRUPT = 4 | (1u << 31),
    IRQ_CAUSE_SUPERVISOR_TIMER_INTERRUPT = 5 | (1u << 31),
    IRQ_CAUSE_MACHINE_TIMER_INTERRUPT = 7 | (1u << 31),
    IRQ_CAUSE_USER_EXTERNAL_INTERRUPT = 8 | (1u << 31),
    IRQ_CAUSE_SUPERVISOR_EXTERNAL_INTERRUPT = 9 | (1u << 31),
    IRQ_CAUSE_MACHINE_EXTERNAL_INTERRUPT = 11 | (1u << 31),

    IRQ_CAUSE_UNKOWN = 0xffffffff
} irq_cause_t;

typedef struct irq_state
{
    regs_t regs;
    uint32_t mepc;
    uint32_t mtval;
    irq_cause_t mcause;
} __attribute__((__packed__)) irq_state_t;

bool irq_init();
void irq_enable();
void irq_disable();
bool irq_is_enabled();

void irq_attach_timer_handler(void (*handler)(regs_t *regs));
void irq_attach_ecall_handler(void (*handler)());

void irq_handler(irq_state_t *state);

#endif