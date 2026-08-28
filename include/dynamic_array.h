#ifndef PD_DYNARR_H
#define PD_DYNARR_H

#include <stddef.h>
#include <stdint.h>

#define PD_ARR_INIT_SIZE 32

typedef struct ArrayHeader
{
    void     *mem;
    size_t   size;
    size_t   cap;
    uint16_t typeSize;
}
ArrayHeader;

#endif
