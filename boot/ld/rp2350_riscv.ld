OUTPUT_FORMAT(elf32-littleriscv)
MEMORY
{
    FLASH(rx) : ORIGIN = 0x10000000, LENGTH = 4M
    RAM(rwx) : ORIGIN = 0x20080000, LENGTH = 8k
}
ENTRY(_entry_point)
SECTIONS
{
    .image : {
        . = ALIGN(4);
        KEEP (*(.image_def))
        . = ALIGN(4);
    } > FLASH

    .text : {
        . = ALIGN(4);
        KEEP (*(.reset))
        . = ALIGN(4);
        *(.text*)
        . = ALIGN(4);
    } > FLASH

    .rodata : {
        . = ALIGN(4);
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    .data : {
        . = ALIGN(4);
        __data_start = .;
        *(.data*)
        . = ALIGN(4);
        *(.sdata*)
        . = ALIGN(4);
        __data_end = .;
        . = ALIGN(4);
    } > RAM AT> FLASH

    __data_pointer = LOADADDR(.data);

    .bss (NOLOAD) : {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss*)
        . = ALIGN(4);
        *(.sbss*)
    } > RAM

    __bss_end = ORIGIN(RAM) + LENGTH(RAM);
    __stack_pointer = __bss_end;
}