#include <stdio.h>

#include "dynamic_array.h"

void pdPushArr
(
    DynArr *dyn,
    void   *element
){
    if(!dyn)
    {
        fprintf(stderr, "\033[31;1mERROR: cannot push to nullptr Dynamic Array."
                "\033[0m\n");
        return;
    }
    else if(!dyn->typeSize)
    {
        fprintf(stderr, "\033[31;1mERROR: typeSize cannot be NULL.\033[0m\n");
        return;
    }

    if(!dyn->reserved)
    {
        // TODO: alloc
    }
    else if(dyn->size + 1 > dyn->reserved)
    {
        // TODO: realloc to dyn->reserved * 2
    }
}

void pdRemoveArr
(
    DynArr *dyn,
    void   *element
){
    // TODO: remove specified element from array, which means moving the tail end over
    // it
}

void pdReserveArr
(
    DynArr   *dyn,
    uint64_t count
){
    // TODO: alloc like push
}

void pdClearArr
(
    DynArr *dyn
){
    // TODO: free? set to 0? we'll see
}
