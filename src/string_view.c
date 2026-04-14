#include "string_view.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StringView cstr_sv
(
    const char *cstr
){
    return(StringView)
    {
        .data = cstr,
        .size = strlen(cstr)
    };
}

const char *sv_cstr
(
    StringView *sv
){
    char *result = malloc(sv->size + 1);
    memcpy(result, sv->data, sv->size);
    result[sv->size] = '\0';

    return result;
}

StringView sv_substr
(
    StringView *sv,
    size_t     start_pos,
    size_t     end_pos
){
    // TODO:
}

void sv_trim
(
    StringView *sv,
    size_t     count,
    uint8_t    direction
){
    if(direction > SV_BOTH)
    {
        fprintf(stderr, "\033[31;3;1mERROR: unknown direction.\033[0m\n");
        return;
    }

    if(direction == SV_LEFT || direction == SV_BOTH)
    {
        size_t i = count;
        if(i > sv->size)
        {
            i = sv->size;
        }
        sv->size -= i;
        sv->data += i;
    }
    if(direction == SV_RIGHT || direction == SV_BOTH)
    {
        size_t i = count;
        if(i > sv->size)
        {
            i = sv->size;
        }
        sv->size -= i;
    }
}

extern uint8_t sv_comp
(
    StringView *first,
    StringView *second
){
    if(first->size == 0 && second->size == 0)
    {
        return SV_SAME;
    }
    else if(first->size == 0 || second->size == 0)
    {
        return SV_DIFFERENT;
    }

    // second->size == 3
    // i = 2
    // sec
    // sec
    //   ^

    size_t i = 0;
    for(; i < first->size; ++i)
    {
        if(first->data[i] != second->data[i])
        {
            return SV_DIFFERENT;
        }
        if(i + 1 == second->size && i + 1 < first->size)
        {
            return SV_ISSUBSTR_RIGHT;
        }
    }

    if(i == second->size)
    {
        return SV_SAME;
    }
    return SV_ISSUBSTR_LEFT;
}
