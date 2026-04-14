#include <string_view.h>

StringView cstr_sv
(
    const char *cstr
){
    // TODO:
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
    if(direction == SV_LEFT || direction == SV_BOTH)
    {
        for(size_t i = count; i > 0; --i)
        {
            if(sv->size == 0)
            {
                return;
            }
            ++sv->data;
            --sv->size;
        }
    }
    if(direction == SV_RIGHT || direction == SV_BOTH)
    {
        for(size_t i = count; i > 0; --i)
        {
            if(sv->size == 0)
            {
                return;
            }
            --sv->size;
        }
    }
}
