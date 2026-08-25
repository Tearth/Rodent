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

# |--------------------------------------------------|
# | Byte | Value           | Description             |
# |--------------------------------------------------|
# | 0    | 0x44            | Block Type: ENTRY_POINT |
# | 1    | 0x03            | Block Size: 3 words     |
# | 2-3  | 0x0000          | Pad                     |
# | 4-7  | _reset_handler  | Initial Program Counter |
# | 8-11 | __stack_pointer | Initial Stack Pointer   |
# |--------------------------------------------------|

.byte   0x44
.byte   0x03
.hword  0x00
.word   _reset_handler
.word   __stack_pointer

blocks_end:

# |--------------------------------------------|
# | Byte | Value | Description                 |
# |--------------------------------------------|
# | 0    | 0xff  | Block Type: BLOCK_ITEM_LAST |
# | 1-2  | size  | Block Size: 3 words         |
# | 3    | 0x00  | Pad                         |
# | 4    | 0x00  | Header Position             |
# |--------------------------------------------|

.byte   0xff
.hword  (blocks_end - blocks_begin) / 4
.byte   0x00
.word   0x00

# Footer
.word 0xab123579
