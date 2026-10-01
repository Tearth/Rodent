#ifndef SHARED_MACRO_H
#define SHARED_MACRO_H

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define NOP() __asm__ ("")
#define HALT() while (1) NOP();
#define LEN(a) (sizeof(a) / sizeof(a[0]))

#endif