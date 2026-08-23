#ifndef ELF_H
#define ELF_H

#include <stdint.h>

#define ELF_MAGIC 0x464c457f
#define ELF_TYPE_EXEC 2

#ifdef ARCH_RISCV
#define ELF_MACHINE 243
#endif

typedef struct elf_header
{
    uint32_t signature;
    uint8_t format;
    uint8_t endianness;
    uint8_t version1;
    uint8_t abi;
    uint64_t pad;
    uint16_t type;
    uint16_t machine;
    uint32_t version2;
    uint32_t entry;
    uint32_t phoff;
    uint32_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
} __attribute__((__packed__)) elf_header_t;

typedef struct elf_pheader
{
    uint32_t type;
    uint32_t offset;
    uint32_t vaddr;
    uint32_t paddr;
    uint32_t fsize;
    uint32_t msize;
    uint32_t flags;
    uint32_t align;
} __attribute__((__packed__)) elf_pheader_t;

#endif