#include <stdio.h>

#include "dynamic_array.h"

// beware of not passing a pointer in arr that does not
// have the array header at ptr - 1.
#define pdReserveArr(arr, requested)                                      \
do                                                                        \
{                                                                         \
    if(arr && (requested) < arr.cap)                                      \
    {                                                                     \
        break;                                                            \
    }                                                                     \
                                                                          \
    ArrayHeader *header = 0;                                              \
    if(arr)                                                               \
    {                                                                     \
        header = (ArrayHeader*)arr - 1;                                   \
    }                                                                     \
                                                                          \
    size_t alloc = PD_ARR_INIT_SIZE;                                      \
    if(header && header.cap)                                              \
    {                                                                     \
        alloc = header.cap;                                               \
    }                                                                     \
                                                                          \
    if(alloc < (requested))                                               \
    {                                                                     \
        alloc = (requested);                                              \
    }                                                                     \
                                                                          \
    header = realloc(header, alloc * sizeof(*arr) + sizeof(ArrayHeader)); \
    header->cap = allocated;                                              \
    arr = (void*)(header + 1);                                            \
}                                                                         \
while(0)

// #define pdPushArr(arr, element) \
// do \
// {  \
    // TODO: reserve size * 2 if size not big enough
    // if(!arr)
    // {
    //     fprintf(stderr, "\033[31;1mERROR: cannot push to nullptr Dynamic Array."
    //             "\033[0m\n");
    //     return;
    // }

    // TODO: push element
// } \
// while(0)

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
