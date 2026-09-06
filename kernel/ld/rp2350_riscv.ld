OUTPUT_FORMAT(elf32-littleriscv)
ENTRY(_entry_point)
SECTIONS
{
    . = BASE_ADDR;

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
    }

    /* Fill the rest of bss section, so the kernel size will be a power of 2 */
    __filling = (1 << LOG2CEIL(. - 0x20000000));

    .bss (NOLOAD) : {
        . = ALIGN(__filling);
    }

    __kernel_start = BASE_ADDR;
    __kernel_end = .;

    __stack_pointer = .;
}