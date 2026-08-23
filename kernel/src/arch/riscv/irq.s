.altmacro

.macro store_regs from, to
sw x\from, \from*4(sp)
.if \from<\to
    store_regs %from+1,\to
.endif
.endm

.macro load_regs from, to
lw x\from, \from*4(sp)
.if \from<\to
    load_regs %from+1,\to
.endif
.endm

.global _irq_khandler_entry
.align 2

# Input: none
# Output: none
_irq_khandler_entry:
    addi    sp, sp, -140
    store_regs 0, 31

    csrr    t0, mepc
    csrr    t1, mtval
    csrr    t2, mcause
    sw      t0, 128(sp)
    sw      t1, 132(sp)
    sw      t2, 136(sp)

    mv      a0, sp
    call    irq_handler

    lw      t0, 128(sp)
    lw      t1, 132(sp)
    lw      t2, 136(sp)
    csrw    mepc, t0
    csrw    mtval, t1
    csrw    mcause, t2

    load_regs 0, 31
    addi    sp, sp, 140
    mret
