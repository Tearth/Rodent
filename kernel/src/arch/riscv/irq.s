.altmacro

.macro store_regs current, to
.if \current==2
    csrr x1, mscratch
    sw x1, \current*4(sp)
.else
    sw x\current, \current*4(sp)
.endif
.if \current<\to
    store_regs %current+1,\to
.endif
.endm

.macro load_regs current, to
.if \current!=2
    lw x\current, \current*4(sp)
.endif
.if \current<\to
    load_regs %current+1,\to
.endif
.endm

.global _irq_handler_entry
.align 2

# Input: none
# Output: none
_irq_handler_entry:
    csrw    mscratch, sp
    addi    sp, sp, -144

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
    csrw    mepc, t0

    load_regs 0, 31

    addi    sp, sp, 144
    mret
