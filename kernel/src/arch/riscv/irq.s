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
    # Store SP before it's modified
    csrw    mscratch, sp

    # Switch SP to the kernel one
    la      sp, __stack_pointer
    addi    sp, sp, -144

    # Store all registers except SP (it's read from MSCRATCH)
    store_regs 1, 31

    # Store MEPC, MTVAL, MCAUSE
    csrr    t0, mepc
    csrr    t1, mtval
    csrr    t2, mcause
    sw      t0, 0(sp)
    sw      t0, 128(sp)
    sw      t1, 132(sp)
    sw      t2, 136(sp)

    mv      a0, sp
    call    irq_handler

    # Restore MEPC
    lw      t0, 128(sp)
    csrw    mepc, t0

    # Restore all registers except SP
    load_regs 1, 31

    # Switch SP to the user one
    csrr sp, mscratch

    mret
