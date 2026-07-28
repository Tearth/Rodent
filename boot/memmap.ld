OUTPUT_FORMAT(elf32-littleriscv)
MEMORY
{
    BOOT(rx) : ORIGIN = 0x10000000, LENGTH = 1M
    FS(rw) : ORIGIN = 0x10100000, LENGTH = 3M
    RAM(rwx) : ORIGIN = 0x20000000, LENGTH = 520k
}
ENTRY(_entry_point)
SECTIONS
{
    .image : {
        . = ALIGN(4);
        KEEP (*(.image_def))
        . = ALIGN(4);
    } > BOOT

    .text : {
        . = ALIGN(4);
        KEEP (*(.reset))
        . = ALIGN(4);
        *(.text*)
        . = ALIGN(4);
    } > BOOT

    .rodata : {
        . = ALIGN(4);
        *(.rodata*)
        . = ALIGN(4);
    } > BOOT

    .data : {
        . = ALIGN(4);
        __data_start = .;
        *(.data*)
        . = ALIGN(4);
        *(.sdata*)
        . = ALIGN(4);
        __data_end = .;
    } > RAM AT> BOOT

    __data_pointer = LOADADDR(.data);

    .bss (NOLOAD) : {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss*)
        . = ALIGN(4);
        *(.sbss*)
        . = ALIGN(4);
        __bss_end = .;
    } > RAM

    .filesystem : {
        KEEP (*(.filesystem))
    } > FS
}