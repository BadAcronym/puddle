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

#define pdArr(type) type *
#define pdArrHeader(arr) ((ArrayHeader*)arr - 1)
#define pdArrSize(arr) (pdArrHeader(arr)->size)
#define pdArrCap(arr)  (pdArrHeader(arr)->cap)

// make sure to pass a pointer that is either null, or a correctly constructed array
// that has an array header at ptr - 1.
#define pdArrReserve(arr, requested)                                            \
do                                                                              \
{                                                                               \
    if(arr && (requested) < pdArrCap(arr))                                      \
    {                                                                           \
        break;                                                                  \
    }                                                                           \
                                                                                \
    ArrayHeader *header = 0;                                                    \
    if(arr)                                                                     \
    {                                                                           \
        header = pdArrHeader(arr);                                              \
    }                                                                           \
                                                                                \
    size_t alloc = (requested);                                                 \
    if(requested <= 0)                                                          \
    {                                                                           \
        alloc = PD_ARR_INIT_SIZE;                                               \
    }                                                                           \
                                                                                \
    if(header && header->cap)                                                   \
    {                                                                           \
        alloc = header->cap;                                                    \
    }                                                                           \
                                                                                \
    header = realloc(header, alloc * sizeof(*arr) + sizeof(ArrayHeader));       \
    header->cap = alloc;                                                        \
    arr = (void*)(header + 1);                                                  \
}                                                                               \
while(0)

#define pdArrPush(arr, elem)                                                \
do                                                                          \
{                                                                           \
    size_t ogSize = 0;                                                      \
                                                                            \
    if(!arr)                                                                \
    {                                                                       \
        pdArrReserve(arr, PD_ARR_INIT_SIZE);                                \
    }                                                                       \
    else if(pdArrSize(arr) + 1 > pdArrCap(arr))                             \
    {                                                                       \
        pdArrReserve(arr, pdArrCap(arr) * 2);                               \
    }                                                                       \
                                                                            \
    arr[pdArrSize(arr)] = elem;                                             \
    pdArrSize(arr)++;                                                       \
                                                                            \
}                                                                           \
while(0)

// void pdRemoveArr
// (
//     DynArr *arr,
//     void   *element
// ){
    // TODO: remove specified element from array, which means moving the
    // tail end over it
    // TODO: specify by index, not by value
// }

// void pdClearArr
// (
//     DynArr *arr
// ){
    // TODO: free? set to 0? we'll see
// }

#endif
