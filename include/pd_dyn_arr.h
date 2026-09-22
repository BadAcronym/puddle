#ifndef PD_DYNARR_H
#define PD_DYNARR_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define PD_ARR_INIT_SIZE 32

typedef struct ArrayHeader
{
    size_t size;
    size_t cap;
}
ArrayHeader;

// take care to declare 0, not some garbage pointer.
#define pdArr(type) type *

#define pdArrHeader(arr) ((ArrayHeader*)arr - 1)
#define pdArrSize(arr) (pdArrHeader(arr)->size)
#define pdArrCap(arr)  (pdArrHeader(arr)->cap)

#define pdArrFree(arr) free(pdArrHeader(arr))

// make sure to pass a pointer that is either null, or a correctly constructed array
// that has an array header at ptr - 1.
#define pdArrReserve(arr, requested)                                                   \
do                                                                                     \
{                                                                                      \
    if(arr && (requested) < pdArrCap(arr))                                             \
    {                                                                                  \
        break;                                                                         \
    }                                                                                  \
                                                                                       \
    ArrayHeader *header = 0;                                                           \
    uint8_t     init    = 0;                                                           \
    if(arr)                                                                            \
    {                                                                                  \
        header = pdArrHeader(arr);                                                     \
    }                                                                                  \
    else                                                                               \
    {                                                                                  \
        init = 1;                                                                      \
    }                                                                                  \
                                                                                       \
    size_t alloc = (requested);                                                        \
    if(requested <= 0)                                                                 \
    {                                                                                  \
        alloc = PD_ARR_INIT_SIZE;                                                      \
    }                                                                                  \
                                                                                       \
    header = realloc(header, alloc * sizeof(*arr) + sizeof(ArrayHeader));              \
    header->cap = alloc;                                                               \
    if(init)                                                                           \
    {                                                                                  \
        header->size = 0;                                                              \
    }                                                                                  \
    arr = (void*)(header + 1);                                                         \
}                                                                                      \
while(0)

#define pdArrPush(arr, elem)                                                           \
do                                                                                     \
{                                                                                      \
    if(!arr)                                                                           \
    {                                                                                  \
        pdArrReserve(arr, PD_ARR_INIT_SIZE);                                           \
    }                                                                                  \
    else if(pdArrSize(arr) + 1 > pdArrCap(arr))                                        \
    {                                                                                  \
        pdArrReserve(arr, pdArrCap(arr) * 2);                                          \
    }                                                                                  \
                                                                                       \
    arr[pdArrSize(arr)] = elem;                                                        \
    ++pdArrSize(arr);                                                                  \
}                                                                                      \
while(0)

#define pdArrRemove(arr, index)                                                        \
do                                                                                     \
{                                                                                      \
    if(!arr || !pdArrSize(arr) || (index) > pdArrSize(arr) - 1)                        \
    {                                                                                  \
        break;                                                                         \
    }                                                                                  \
                                                                                       \
    if(index == pdArrSize(arr) - 1)                                                    \
    {                                                                                  \
        arr[index] = 0;                                                                \
        break;                                                                         \
    }                                                                                  \
                                                                                       \
    size_t moveSize = (pdArrSize(arr) - (index) - 1) * sizeof(*arr);                   \
    memmove(&arr[(index)], &arr[(index) + 1], moveSize);                               \
    --pdArrSize(arr);                                                                  \
}                                                                                      \
while(0)                                                                               \

#endif
