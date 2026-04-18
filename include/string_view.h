#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// sv_trim directions
#define SV_LEFT  0L
#define SV_RIGHT 1L
#define SV_BOTH  2L

// sv_comp results
#define SV_SAME            0L
#define SV_LONGER_FIRST    1L
#define SV_LONGER_SECOND   2L
#define SV_SUBSTR_FIRST    3L
#define SV_SUBSTR_SECOND   4L
#define SV_DIFFERENT       5L

// macros for printf() formatting and printing.
#define PRI_SV "%.*s"
#define ARG_SV(sv) (int)(sv).size, (sv).data

// length-based string.
// data is not guaranteed to be null-terminated.
// use sv_cstr() if you require access to a cstring.
typedef struct sv
{
    const char *data;
    size_t     size;
}
StringView;

// will create a stringview from a cstring.
// will use strlen() to figure out the length of the string.
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

// safe access for anything requiring a null-terminated cstring,
// because we can't be sure that the stringview is going to be null-terminated.
// will use malloc() to give you a new const char *.
const char *sv_cstr
(
    StringView *sv
){
    char *result = malloc(sv->size + 1);
    memcpy(result, sv->data, sv->size);
    result[sv->size] = '\0';

    return result;
}

// will return the trimmed substring as a new stringview.
// start_pos & end_pos are both positions in the original string.
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

// will trim count characters from: SV_LEFT, SV_RIGHT or SV_BOTH.
// in the case of both, it will first trim count from the left,
// then try to trim count from the right.
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

// will return:
// 0: `SV_SAME` if the stringviews have the same content and are the same length.
// 2: `SV_LONGER_FIRST`, if the second sv is contained fully in the first,
// but the first sv is longer.
// 1: `SV_LONGER_SECOND`, analogous to `SV_LONGER_FIRST`, but vice-versa.
// 3: `SV_DIFFERENT`, if they are different stringviews.
// NOTE: will return `true` for strings "test" and "test2",
// but `false` for strings "test" and "2test". For true substring testing, use
// sv_is_substr instead.
uint8_t sv_comp
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

// TODO: will return:
// 3: `SV_DIFFERENT`, if they are different stringviews.
// 4: `SV_SUBSTR_FIRST`, if the first stringview is contained entirely in the second
// 5: `SV_SUBSTR_SECOND`, analogous to `SV_SUBSTR_FIRST`, but vice-versa.
uint8_t sv_is_substr
(
    StringView *first,
    StringView *second
){
    // TODO: substr testing :P
    return SV_DIFFERENT;
}

// concatenates `first` and `second` into `result` one after the other.
// `first`  + `second` = `result`.
// "Hello " + "World"  = "Hello World".
void sv_concat
(
    StringView *first,
    StringView *second,
    StringView *result
){
    result->size = first->size + second->size;
    result->data = malloc(first->size + second->size);
    memcpy((void*)result->data, (void*)first->data, first->size);
    memcpy((void*)(result->data + first->size), (void*)second->data, second->size);
}

#endif
