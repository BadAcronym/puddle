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
    if(start_pos > end_pos)
    {
        fprintf(stderr, "\033[31;3;1mERROR: end_pos cannot be smaller than start_pos.\033[0m\n");
        return(StringView)
        {
            .data = 0,
            .size = 0
        };
    }

    if(start_pos > sv->size)
    {
        fprintf(stderr, "\033[31;3;1mERROR: start_pos cannot be larger than sv->size.\033[0m\n");
        return(StringView)
        {
            .data = 0,
            .size = 0
        };
    }

    if(end_pos > sv->size)
    {
        end_pos = sv->size;
    }

    StringView result;
    result.data = sv->data + start_pos;
    result.size = sv->size - start_pos - (sv->size - end_pos);

    if(start_pos == end_pos)
    {
        result.size = 1;
    }

    return result;
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

    size_t i = 0;
    for(; i < first->size; ++i)
    {
        if(first->data[i] != second->data[i])
        {
            return SV_DIFFERENT;
        }
        if(i + 1 == second->size && i + 1 < first->size)
        {
            return SV_LONGER_FIRST;
        }
    }

    if(i == second->size)
    {
        return SV_SAME;
    }
    return SV_LONGER_SECOND;
}

extern uint8_t sv_is_substr
(
    StringView *first,
    StringView *second
){
    // TODO: substr testing :P
    return SV_DIFFERENT;
}
