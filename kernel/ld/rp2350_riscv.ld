OUTPUT_FORMAT(elf32-littleriscv)
ENTRY(kmain)
SECTIONS
{
    . = 0x20000000;

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
        . = ALIGN(4);
    }

    /* Fill the rest of bss section, so the kernel size will be a power of 2 */
    __filling = (1 << LOG2CEIL(. - 0x20000000));

    .bss (NOLOAD) : {
        . = ALIGN(__filling);
    }

    __stack_pointer = .;
}