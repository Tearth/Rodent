OUTPUT_FORMAT(elf32-littleriscv)
ENTRY(_entry_point)
SECTIONS
{
    . = 0x00000000;

    .text : {
        . = ALIGN(4);
        *(.text*)
        . = ALIGN(4);
    }

    .rodata : {
        . = ALIGN(4);
        *(.rodata*)
        . = ALIGN(4);
    }

    .data : {
        . = ALIGN(4);
        *(.data*)
        . = ALIGN(4);
        *(.sdata*)
        . = ALIGN(4);
    }

    .bss (NOLOAD) : {
        . = ALIGN(4);
        *(.bss*)
        . = ALIGN(4);
        *(.sbss*)
        . = ALIGN(16);
        . += STACK_SIZE;
        . = ALIGN(CELL_SIZE);
    }

    __stack_pointer = .;
}