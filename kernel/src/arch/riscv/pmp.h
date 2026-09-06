#ifndef PMP_H
#define PMP_H

#include <stdint.h>

#define PMP_REGION0_RWX_SHIFT 0
#define PMP_REGION0_RWX_MASK 7u << 0
#define PMP_REGION0_MODE_SHIFT 3
#define PMP_REGION0_MODE_MASK 3u << 3

#define PMP_REGION1_RWX_SHIFT 8
#define PMP_REGION1_RWX_MASK 7u << 8
#define PMP_REGION1_MODE_SHIFT 11
#define PMP_REGION1_MODE_MASK 3u << 11

#define PMP_REGION2_RWX_SHIFT 16
#define PMP_REGION2_RWX_MASK 7u << 16
#define PMP_REGION2_MODE_SHIFT 19
#define PMP_REGION2_MODE_MASK 3u << 19

#define PMP_REGION3_RWX_SHIFT 24
#define PMP_REGION3_RWX_MASK 7u << 24
#define PMP_REGION3_MODE_SHIFT 27
#define PMP_REGION3_MODE_MASK 3u << 27

#define PMP_REGION4_RWX_SHIFT 0
#define PMP_REGION4_RWX_MASK 7u << 0
#define PMP_REGION4_MODE_SHIFT 3
#define PMP_REGION4_MODE_MASK 3u << 3

#define PMP_REGION5_RWX_SHIFT 8
#define PMP_REGION5_RWX_MASK 7u << 8
#define PMP_REGION5_MODE_SHIFT 11
#define PMP_REGION5_MODE_MASK 3u << 11

#define PMP_REGION6_RWX_SHIFT 16
#define PMP_REGION6_RWX_MASK 7u << 16
#define PMP_REGION6_MODE_SHIFT 19
#define PMP_REGION6_MODE_MASK 3u << 19

#define PMP_REGION7_RWX_SHIFT 24
#define PMP_REGION7_RWX_MASK 7u << 24
#define PMP_REGION7_MODE_SHIFT 27
#define PMP_REGION7_MODE_MASK 3u << 27

typedef enum pmp
{
    PMP_REGION0,
    PMP_REGION1,
    PMP_REGION2,
    PMP_REGION3,
    PMP_REGION4,
    PMP_REGION5,
    PMP_REGION6,
    PMP_REGION7
} pmp_t;

typedef enum pmp_mode
{
    PMP_MODE_OFF,
    PMP_MODE_TOR,
    PMP_MODE_NA4,
    PMP_MODE_NATOP
} pmp_mode_t;

typedef struct pmp_def
{
    uint32_t rwx_shift;
    uint32_t rwx_mask;
    uint32_t mode_shift;
    uint32_t mode_mask;
} pmp_def_t;

void pmp_set_mode(pmp_t region, pmp_mode_t mode);
pmp_mode_t pmp_get_mode(pmp_t region, pmp_mode_t mode);

bool pmp_set_area(pmp_t region, void *base, uint32_t size);
void pmp_get_area(pmp_t region, void **base, uint32_t *size);

void pmp_set_rwx(pmp_t region, bool r, bool w, bool x);
void pmp_get_rwx(pmp_t region, bool *r, bool *w, bool *x);

#endif