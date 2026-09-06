#include "pmp.h"

static uint32_t pmp_read_pmpcfg(pmp_t region);
static void pmp_write_pmpcfg(pmp_t region, uint32_t pmpcfg);
static uint32_t pmp_read_pmpaddr(pmp_t region);
static uint32_t pmp_read_pmpaddr(pmp_t region);
static void pmp_write_pmpaddr(pmp_t region, uint32_t pmpaddr);

static const pmp_def_t pmp_defs[] =
{
    // PMP_REGION0
    {
        .rwx_shift = PMP_REGION0_RWX_SHIFT,
        .rwx_mask = PMP_REGION0_RWX_MASK,
        .rwx_shift = PMP_REGION0_MODE_SHIFT,
        .mode_mask = PMP_REGION0_MODE_MASK
    },
    // PMP_REGION1
    {
        .rwx_shift = PMP_REGION1_RWX_SHIFT,
        .rwx_mask = PMP_REGION1_RWX_MASK,
        .mode_mask = PMP_REGION1_MODE_SHIFT,
        .mode_mask = PMP_REGION1_MODE_MASK
    },
    // PMP_REGION2
    {
        .rwx_shift = PMP_REGION2_RWX_SHIFT,
        .rwx_mask = PMP_REGION2_RWX_MASK,
        .mode_mask = PMP_REGION2_MODE_SHIFT,
        .mode_mask = PMP_REGION2_MODE_MASK
    },
    // PMP_REGION3
    {
        .rwx_shift = PMP_REGION3_RWX_SHIFT,
        .rwx_mask = PMP_REGION3_RWX_MASK,
        .mode_mask = PMP_REGION3_MODE_SHIFT,
        .mode_mask = PMP_REGION3_MODE_MASK
    },
    // PMP_REGION4
    {
        .rwx_shift = PMP_REGION4_RWX_SHIFT,
        .rwx_mask = PMP_REGION4_RWX_MASK,
        .mode_mask = PMP_REGION4_MODE_SHIFT,
        .mode_mask = PMP_REGION4_MODE_MASK
    },
    // PMP_REGION5
    {
        .rwx_shift = PMP_REGION5_RWX_SHIFT,
        .rwx_mask = PMP_REGION5_RWX_MASK,
        .mode_mask = PMP_REGION5_MODE_SHIFT,
        .mode_mask = PMP_REGION5_MODE_MASK
    },
    // PMP_REGION6
    {
        .rwx_shift = PMP_REGION6_RWX_SHIFT,
        .rwx_mask = PMP_REGION6_RWX_MASK,
        .mode_mask = PMP_REGION6_MODE_SHIFT,
        .mode_mask = PMP_REGION6_MODE_MASK
    },
    // PMP_REGION7
    {
        .rwx_shift = PMP_REGION7_RWX_SHIFT,
        .rwx_mask = PMP_REGION7_RWX_MASK,
        .mode_mask = PMP_REGION7_MODE_SHIFT,
        .mode_mask = PMP_REGION7_MODE_MASK
    },
};

void pmp_set_mode(pmp_t region, pmp_mode_t mode)
{
    const pmp_def_t* region_sel = &pmp_defs[region];
    uint32_t pmpcfg = pmp_read_pmpcfg(region);

    // Set RX_A (Address Matching Type)
    pmpcfg = (pmpcfg & ~region_sel->mode_mask) | (mode << region_sel->mode_shift);

    pmp_write_pmpcfg(region, pmpcfg);
}

pmp_mode_t pmp_get_mode(pmp_t region, pmp_mode_t mode)
{
    const pmp_def_t* region_sel = &pmp_defs[region];

    // Set RX_A (Address Matching Type)
    return (pmp_read_pmpcfg(region) & region_sel->mode_mask) >> region_sel->mode_shift;
}

bool pmp_set_area(pmp_t region, void *addr, uint32_t size)
{
    uint32_t base = (uint32_t)addr;

    if (base < 4 || size < 8)
    {
        return false;
    }

    if ((size & (size - 1)) != 0 || (base % size) != 0)
    {
        return false;
    }

    uint32_t trail = (size >> 3) - 1;
    uint32_t pmpaddr = (base >> 2) | trail;

    pmp_write_pmpaddr(region, pmpaddr);

    return true;
}

void pmp_get_area(pmp_t region, void **base, uint32_t *size)
{
    uint8_t i = 0;
    uint32_t pmpaddr = pmp_read_pmpaddr(region);

    #if defined(MCU_RP2350) && defined(REV_A2)
    pmpaddr |= 0x3;
    #endif

    while ((pmpaddr & 1) == 1)
    {
        pmpaddr >>= 1;
        i++;
    }

    *size = 1u << (i + 3);
    *base = (void*)((pmpaddr >> 1) << (i + 3));
}

void pmp_set_rwx(pmp_t region, bool r, bool w, bool x)
{
    const pmp_def_t* region_sel = &pmp_defs[region];
    uint32_t pmpcfg = pmp_read_pmpcfg(region);

    #if defined(MCU_RP2350) && defined(REV_A2)
    uint32_t rwx = (x | (w << 1) | (r << 2)) << region_sel->rwx_shift;
    #else
    uint32_t rwx = (r | (w << 1) | (x << 2)) << region_sel->rwx_shift;
    #endif

    // Set RX_R, RX_W, RX_X
    pmpcfg = (pmpcfg & ~region_sel->rwx_mask) | rwx;

    pmp_write_pmpcfg(region, pmpcfg);
}

void pmp_get_rwx(pmp_t region, bool *r, bool *w, bool *x)
{
    const pmp_def_t* region_sel = &pmp_defs[region];
    uint32_t pmpcfg = pmp_read_pmpcfg(region);

    // Read RX_R, RX_W, RX_X
    uint32_t rwx = pmpcfg >> region_sel->rwx_shift;

    #if defined(MCU_RP2350) && defined(REV_A2)
    *x = (rwx & (1u << 0)) != 0;
    *w = (rwx & (1u << 1)) != 0;
    *r = (rwx & (1u << 2)) != 0;
    #else
    *r = (rwx & (1u << 0)) != 0;
    *w = (rwx & (1u << 1)) != 0;
    *x = (rwx & (1u << 2)) != 0;
    #endif
}

uint32_t pmp_read_pmpcfg(pmp_t region)
{
    uint32_t pmpcfg;

    switch (region)
    {
        case PMP_REGION0:
        case PMP_REGION1:
        case PMP_REGION2:
        case PMP_REGION3:
        {
           __asm__ volatile (
                "csrr %0, pmpcfg0"
            : "=r"(pmpcfg));

            break;
        }
        case PMP_REGION4:
        case PMP_REGION5:
        case PMP_REGION6:
        case PMP_REGION7:
        {
            __asm__ volatile (
                "csrr %0, pmpcfg1"
            : "=r"(pmpcfg));

            break;
        }
    }

    return pmpcfg;
}

void pmp_write_pmpcfg(pmp_t region, uint32_t pmpcfg)
{
    switch (region)
    {
        case PMP_REGION0:
        case PMP_REGION1:
        case PMP_REGION2:
        case PMP_REGION3:
        {
            __asm__ volatile (
                "csrw pmpcfg0, %0"
            : : "r"(pmpcfg));

            break;
        }
        case PMP_REGION4:
        case PMP_REGION5:
        case PMP_REGION6:
        case PMP_REGION7:
        {
            __asm__ volatile (
                "csrw pmpcfg1, %0"
            : : "r"(pmpcfg));

            break;
        }
    }
}

uint32_t pmp_read_pmpaddr(pmp_t region)
{
    uint32_t pmpaddr;

    switch (region)
    {
        case PMP_REGION0: __asm__ volatile ("csrr %0, pmpaddr0" : "=r"(pmpaddr)); break;
        case PMP_REGION1: __asm__ volatile ("csrr %0, pmpaddr1" : "=r"(pmpaddr)); break;
        case PMP_REGION2: __asm__ volatile ("csrr %0, pmpaddr2" : "=r"(pmpaddr)); break;
        case PMP_REGION3: __asm__ volatile ("csrr %0, pmpaddr3" : "=r"(pmpaddr)); break;
        case PMP_REGION4: __asm__ volatile ("csrr %0, pmpaddr4" : "=r"(pmpaddr)); break;
        case PMP_REGION5: __asm__ volatile ("csrr %0, pmpaddr5" : "=r"(pmpaddr)); break;
        case PMP_REGION6: __asm__ volatile ("csrr %0, pmpaddr6" : "=r"(pmpaddr)); break;
        case PMP_REGION7: __asm__ volatile ("csrr %0, pmpaddr7" : "=r"(pmpaddr)); break;
    }

    return pmpaddr;
}

void pmp_write_pmpaddr(pmp_t region, uint32_t pmpaddr)
{
    switch (region)
    {
        case PMP_REGION0: __asm__ volatile ("csrw pmpaddr0, %0" : : "r"(pmpaddr)); break;
        case PMP_REGION1: __asm__ volatile ("csrw pmpaddr1, %0" : : "r"(pmpaddr)); break;
        case PMP_REGION2: __asm__ volatile ("csrw pmpaddr2, %0" : : "r"(pmpaddr)); break;
        case PMP_REGION3: __asm__ volatile ("csrw pmpaddr3, %0" : : "r"(pmpaddr)); break;
        case PMP_REGION4: __asm__ volatile ("csrw pmpaddr4, %0" : : "r"(pmpaddr)); break;
        case PMP_REGION5: __asm__ volatile ("csrw pmpaddr5, %0" : : "r"(pmpaddr)); break;
        case PMP_REGION6: __asm__ volatile ("csrw pmpaddr6, %0" : : "r"(pmpaddr)); break;
        case PMP_REGION7: __asm__ volatile ("csrw pmpaddr7, %0" : : "r"(pmpaddr)); break;
    }
}