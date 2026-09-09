.section .reset, "ax"
.global _entry_point

# Input: none
# Output: none
_entry_point:
    li  t0, 0x7dfc
    jr  t0

.section .text
.global _reset_handler

# Input: none
# Output: none
_reset_handler:
    la      a0, __data_pointer
    la      a1, __data_start
    la      a2, __data_end
    call    copy_data
    la      a0, __bss_start
    la      a1, __bss_end
    call    clear_bss
    j       main

# Input:
# - a0 - initial source address
# - a1 - initial destination address
# - a2 - final destination address
# Output: none
copy_data_loop:
    lw      a3, (a0)
    sw      a3, (a1)
    addi    a0, a0, 4
    addi    a1, a1, 4
copy_data:
    bltu    a1, a2, copy_data_loop
    ret

# Input:
# - a0 - address from
# - a1 - address to
# Output: none
clear_bss_loop:
    sw      x0, (a0)
    addi    a0, a0, 4
clear_bss:
    bltu    a0, a1, clear_bss_loop
    ret
