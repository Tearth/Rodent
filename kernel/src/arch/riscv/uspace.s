.altmacro

.macro load_regs current, to
.if \current!=10
    lw x\current, \current*4(a0)
.endif
.if \current<\to
    load_regs %current+1,\to
.endif
.endm

.global uspace_enter
.align 2

# Input:
# - a0 - state
# Output: none
uspace_enter:
    # Restore MEPC
    lw      t0, 0(a0)
    csrw    mepc, t0

    # Restore MSTATUS
    # li      t0, 0x1800
    # csrc    mstatus, t0
    li      t0, 0x1888
    csrs    mstatus, t0

    # Restore all registers except ZERO and A0
    load_regs 1, 31

    # Restore A0
    lw a0, 40(a0)

    mret
