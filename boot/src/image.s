########################
### Image Definition ###
########################

.section .image_def, "a"

# Header
.word 0xffffded3

blocks_begin:

# |----------------------------------------------|
# | Byte | Value  | Description                  |
# |----------------------------------------------|
# | 0    | 0x42   | Block Type: IMAGE_DEF        |
# | 1    | 0x01   | Block Size: 1 word           |
# | 2-3  | 0x1101 | Flags: EXE | RISC-V | RP2350 |
# |----------------------------------------------|

.byte   0x42
.byte   0x01
.hword  0x1101

# |-------------------------------------------------|
# | Byte | Value          | Description             |
# |-------------------------------------------------|
# | 0    | 0x44           | Block Type: ENTRY_POINT |
# | 1    | 0x03           | Block Size: 3 word      |
# | 2-3  | 0x0000         | Pad                     |
# | 4-7  | _reset_handler | Initial Program Counter |
# | 8-11 | 0x20082000     | Initial Stack Pointer   |
# |-------------------------------------------------|

.byte   0x44
.byte   0x03
.hword  0x00
.word   _reset_handler
.word   0x20082000

blocks_end:

# |--------------------------------------------|
# | Byte | Value | Description                 |
# |--------------------------------------------|
# | 0    | 0xff  | Block Type: BLOCK_ITEM_LAST |
# | 2-3  | size  | Block Size: 3 word          |
# | 4    | 0x00  | Pad                         |
# | 5    | 0x00  | Header Position             |
# |--------------------------------------------|

.byte   0xff
.hword  (blocks_end - blocks_begin) / 4
.byte   0x00
.word   0

# Footer
.word 0xab123579

#######################
### ELF Entry Point ###
#######################

.section .reset, "ax"
.global _entry_point

# Input: none
# Output: none
_entry_point:
    li  t0, 0x7dfc
    jr  t0

#####################
### Reset Handler ###
#####################

.section .text

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
