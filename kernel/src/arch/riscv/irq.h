#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include <stdlib.h>
#include "../../log.h"

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
    uint32_t zero;
    uint32_t ra;
    uint32_t sp;
    uint32_t gp;
    uint32_t tp;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t s0;
    uint32_t s1;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t a4;
    uint32_t a5;
    uint32_t a6;
    uint32_t a7;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t s8;
    uint32_t s9;
    uint32_t s10;
    uint32_t s11;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;

    uint32_t mepc;
    uint32_t mtval;
    irq_cause_t mcause;
} __attribute__((__packed__)) irq_state_t;

#endif