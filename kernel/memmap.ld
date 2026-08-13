OUTPUT_FORMAT(elf32-littleriscv)
MEMORY
{
    RAM(rwx) : ORIGIN = 0x20000000, LENGTH = 32k
}
ENTRY(main)
SECTIONS
{
    .text : {
        . = ALIGN(4);
        *(.text*)
        . = ALIGN(4);
    } > RAM

    .rodata : {
        . = ALIGN(4);
        *(.rodata*)
        . = ALIGN(4);
    } > RAM

    .data : {
        . = ALIGN(4);
        *(.data*)
        . = ALIGN(4);
        *(.sdata*)
        . = ALIGN(4);
    } > RAM

    __stack_pointer = ORIGIN(RAM) + LENGTH(RAM);

    .bss (NOLOAD) : {
        . = ALIGN(4);
        *(.bss*)
        . = ALIGN(4);
        *(.sbss*)
        . = ALIGN(4);
    } > RAM
}