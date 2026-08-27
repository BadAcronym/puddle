#ifndef PD_DYNARR_H
#define PD_DYNARR_H

#include <stddef.h>
#include <stdint.h>

#define PD_ARR_INIT_SIZE 32

typedef struct DynArr
{
    void     *mem;
    size_t   size;
    size_t   reserved;
    uint16_t typeSize;
}
DynArr;

extern void pdPushArr
(
    DynArr *dyn,
    void   *element
);

extern void pdRemoveArr
(
    DynArr *dyn,
    void   *element
);

extern void pdReserveArr
(
    DynArr   *dyn,
    uint64_t count
);

extern void pdClearArr
(
    DynArr *dyn
);

#endif
