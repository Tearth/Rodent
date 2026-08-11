OUTPUT_FORMAT(elf32-littleriscv)
MEMORY
{
    FLASH(rx) : ORIGIN = 0x10000000, LENGTH = 4M
    RAM(rwx) : ORIGIN = 0x20000000, LENGTH = 520k
}
ENTRY(main)
SECTIONS
{
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
    } > RAM AT> FLASH

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
}