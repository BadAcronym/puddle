#ifndef STRING_VIEW_DECL
#define STRING_VIEW_DECL

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
// use like this, with result & expected both stringviews:
// fprintf(stderr, "expected: " PRI_SV " got: " PRI_SV ", ARG_SV(expected), ARG_SV(result));
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

// will create a stringview from a cstring, by passing the pointer given
// as its data and counting its length.
extern StringView cstr_sv
(
    const char *cstr
);

// will create a stringview, but will not just pass a pointer. Will actually
// copy its contents and allocate to a new pointer.
StringView cstr_sv_new
(
    const char *cstr
);

// safe access for anything requiring a null-terminated cstring,
// because we can't be sure that the stringview is going to be null-terminated.
// will use malloc() to give you a new const char *.
extern const char *sv_cstr
(
    StringView *sv
);

// will return the trimmed substring as a new stringview.
// start_pos & end_pos are both positions in the original string.
extern StringView sv_substr
(
    StringView *sv,
    size_t     start_pos,
    size_t     end_pos
);

// will trim count characters from: `SV_LEFT`, `SV_RIGHT` or `SV_BOTH`.
// in the case of both, it will first trim `count` from the left,
// then try to trim `count` from the right.
extern void sv_trim
(
    StringView *sv,
    size_t     count,
    uint8_t    direction
);

// will return:
// 0: `SV_SAME` if the stringviews have the same content and are the same length.
// 2: `SV_LONGER_FIRST`, if the second sv is contained fully in the first,
// but the first sv is longer.
// 1: `SV_LONGER_SECOND`, if the first sv is contained fully in the second,
// but the second sv is longer.
// 3: `SV_DIFFERENT`, if they are different stringviews.
// NOTE: will return `true` for strings "test" and "test2",
// but `false` for strings "test" and "2test". For true substring testing, use
// sv_is_substr instead.
extern uint8_t sv_comp
(
    StringView *first,
    StringView *second
);

// TODO: will return:
// 3: `SV_DIFFERENT`, if they are different stringviews.
// 4: `SV_SUBSTR_FIRST`, if the first stringview is contained entirely in the second
// 5: `SV_SUBSTR_SECOND`, analogous to `SV_SUBSTR_FIRST`, but vice-versa.
extern uint8_t sv_is_substr
(
    StringView *first,
    StringView *second
);

// concatenates `first` and `second` one after the other.
// `first`  + `second` = `result`.
// "Hello " + "World"  = "Hello World".
extern const char *sv_concat
(
    StringView *first,
    StringView *second
);

// will add a single character to the desired stringview at the desired location(s).
// side can be `SV_LEFT`, `SV_RIGHT` or `SV_BOTH`.
extern const char *sv_add_char
(
    StringView *sv,
    char       c,
    uint8_t    side
);
#endif

#ifdef STRING_VIEW_IMPL
StringView cstr_sv
(
    const char *cstr
){
    uint32_t i = 0;
    for(; cstr[i] != '\0'; ++i)
    {
    }

    return(StringView)
    {
        .data = cstr,
        .size = i
    };
}

StringView cstr_sv_new
(
    const char *cstr
){
    uint32_t i = 0;
    for(; cstr[i] != '\0'; ++i)
    {
    }

    char *buf = malloc(i + 1);
    memcpy((void*)buf, cstr, i);
    buf[i + 1] = '\0';

    return(StringView)
    {
        .data = buf,
        .size = i
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

uint8_t sv_is_substr
(
    StringView *first,
    StringView *second
){
    // TODO: substr testing :P
    return SV_DIFFERENT;
}

// URGENT: test sv_concat with pointer aliases
const char *sv_concat
(
    StringView *first,
    StringView *second
){
    if(!first || !second)
    {
        fprintf(stderr, "\033[31mERROR: bad sv input.\033[0m\n");
        return 0;
    }

    if(!first->data || !second->data)
    {
        fprintf(stderr, "\033[31mERROR: bad stringview data pointer.\033[0m\n");
        return 0;
    }

    const char *first_data  = sv_cstr(first);
    const char *second_data = sv_cstr(second);

    char *result = malloc(first->size + second->size + 1);
    memcpy((void*)result, (void*)first_data, first->size);
    memcpy((void*)(result + first->size), (void*)second_data, second->size);
    result[first->size + second->size] = '\0';

    free((void*)first_data);
    free((void*)second_data);

    return result;
}

const char *sv_add_char
(
    StringView *sv,
    char       c,
    uint8_t    side
){
    char *data = malloc(sv->size + 3);

    if(side == SV_RIGHT)
    {
        memcpy(data, sv->data, sv->size);
        data[sv->size]     = c;
        data[sv->size + 1] = '\0';
    }
    else if(side == SV_LEFT)
    {
        memcpy(data + 1, sv->data, sv->size);
        data[0] = c;
        data[sv->size + 1] = '\0';
    }
    else if(side == SV_BOTH)
    {
        memcpy(data + 1, sv->data, sv->size);
        data[0] = c;
        data[sv->size + 1] = c;
        data[sv->size + 2] = '\0';
    }

    return data;
}

#endif
