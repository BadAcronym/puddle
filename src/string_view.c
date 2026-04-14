#include <stdio.h>
#include <string.h>
#include <string_view.h>

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
    const char *sv
){
    // TODO:
}

StringView sv_substr
(
    size_t start_pos,
    size_t end_pos
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
    // TODO: compare strings

    return SV_DIFFERENT;
}
