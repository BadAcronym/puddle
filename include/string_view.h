#ifndef STRING_VIEW_DECL
#define STRING_VIEW_DECL

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SV_LEFT  0L
#define SV_RIGHT 1L
#define SV_BOTH  2L

#define SV_DIFFERENT     0L
#define SV_SAME          1L
#define SV_IS_SUBSTR     2L

// macros for `printf()` formatting and printing.
// use like this, with result & expected both stringviews:
// fprintf(stderr, "expected: "PRI_SV" got: "PRI_SV", ARG_SV(expected), ARG_SV(result));
#define PRI_SV "%.*s"
#define ARG_SV(sv) (int)(sv).size, (sv).data

// length-based string.
// data is not guaranteed to be null-terminated.
// use `sv_cstr()` if you require access to a cstring.
// tip: just cast to `StringView` if you require using puddle's sv functions.
typedef struct s
{
    char   *data;
    size_t size;
}
String;

// length-based string view (const pointer).
// data is `not` guaranteed to be null-terminated.
// use `sv_cstr()` if you require access to a cstring.
typedef struct sv
{
    const char *data;
    size_t     size;
}
StringView;

// will create a string from a cstring, by passing the pointer given
// as its data and counting its length.
extern String cstr_str
(
    char *cstr
);

// will create a string, but will not just pass a pointer. Will actually
// copy its contents and allocate to a new pointer.
String cstr_scpy
(
    char *cstr
);

// will create a stringview from a cstring, by passing the pointer given
// as its data and counting its length.
extern StringView cstr_sv
(
    const char *cstr
);

// will create a stringview, but will not just pass a pointer. Will actually
// copy its contents and allocate to a new pointer.
StringView cstr_sv_cpy
(
    const char *cstr
);

// safe access for anything requiring a null-terminated cstring,
// because we can't be sure that the stringview is going to be null-terminated.
// will use malloc() to give you a new const char *.
extern const char *sv_cstr
(
    StringView sv
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

// Will return:
// 0: `SV_DIFFERENT`, if they are different stringviews.
// 1: `SV_SAME`, if the stringviews have the same content and are the same length.
// In this case, the strings "test" and "test2" are not the same string. For substring
// testing, use `sv_is_substr`.
extern uint8_t sv_is_same
(
    StringView first,
    StringView second
);

// Will return:
// 0: `SV_DIFFERENT`, if the `first` string is `not` included in the `second` string.
// 1: `SV_SAME`, if the stringviews have the same content and are the same length.
// 2: `SV_IS_SUBSTR`, if the `first` string is `fully` included in the `second` string.
extern uint8_t sv_is_substr
(
    StringView first,
    StringView second
);

// Will return a pointer to the start of `pattern` inside `sv`, if it was found.
// If it wasn't found, the pointer is null.
extern const char *sv_find
(
    StringView pattern,
    StringView sv
);

// Will return a StringView that finds the `i-th` element in `sv`, where an element is
// a substring that starts with either the start of the string or a `delim` character
// and ends with either the end of the string or a `delim` character. E.g., in the
// string: `"hello;test;path;for;example"`, and `delim = ';'`, `i = 0` would yield
// "hello", `i = 3` yields "for", etc.
StringView sv_find_by_delim
(
    StringView sv,
    char       delim,
    uint32_t   index
);

// concatenates `first` and `second` one after the other.
// `first`  + `second` = `result`.
// "Hello " + "World"  = "Hello World".
extern const char *sv_concat
(
    StringView first,
    StringView second
);
#endif

#ifdef STRING_VIEW_IMPL
String cstr_str
(
    char *cstr
){
    uint32_t i = 0;
    for(; cstr[i] != '\0'; ++i)
    {
    }

    return(String)
    {
        .data = cstr,
        .size = i
    };
}

String cstr_scpy
(
    char *cstr
){
    uint32_t i = 0;
    for(; cstr[i] != '\0'; ++i)
    {
    }

    char *buf = malloc(i + 2);
    memcpy((void*)buf, cstr, i);
    buf[i + 1] = '\0';

    return(String)
    {
        .data = buf,
        .size = i
    };
}

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

StringView cstr_sv_cpy
(
    const char *cstr
){
    uint32_t i = 0;
    for(; cstr[i] != '\0'; ++i)
    {
    }

    char *buf = malloc(i + 2);
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
    StringView sv
){
    char *result = malloc(sv.size + 1);
    memcpy(result, sv.data, sv.size);
    result[sv.size] = '\0';

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
        fprintf(stderr, "\033[31;3;1mERROR: end_pos cannot be smaller than start_pos."
                "\033[0m\n");
        return(StringView)
        {
            .data = 0,
            .size = 0
        };
    }

    if(start_pos > sv->size)
    {
        fprintf(stderr, "\033[31;3;1mERROR: start_pos cannot be larger than sv->size."
                "\033[0m\n");
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

uint8_t sv_same
(
    StringView first,
    StringView second
){
    if((first.data == second.data) || (first.size == 0 && second.size == 0))
    {
        return SV_SAME;
    }
    else if(first.size == 0 || second.size == 0)
    {
        return SV_DIFFERENT;
    }

    size_t i = 0;
    for(; i < first.size && i < second.size; ++i)
    {
        if(first.data[i] != second.data[i])
        {
            return SV_DIFFERENT;
        }
    }

    if(i == second.size && i == first.size)
    {
        return SV_SAME;
    }
    return SV_DIFFERENT;
}

uint8_t sv_is_substr
(
    StringView first,
    StringView second
){
    if((first.data == second.data) || (first.size == 0 && second.size == 0))
    {
        return SV_SAME;
    }

    for(size_t i = 0; i < second.size; ++i)
    {
        size_t j = 0;
        for(; j < first.size; ++j)
        {
            if(second.data[i + j] != first.data[j])
            {
                break;
            }
        }

        if(j == first.size)
        {
            return SV_IS_SUBSTR;
        }
    }

    return SV_DIFFERENT;
}

const char *sv_find
(
    StringView pattern,
    StringView sv
){
    if(sv.data == 0 || pattern.data == 0 || sv.size == 0 || pattern.size == 0)
    {
        return 0;
    }

    for(size_t i = 0; i < sv.size; ++i)
    {
        size_t j = 0;
        for(; j < pattern.size; ++j)
        {
            if(sv.data[i + j] != pattern.data[j])
            {
                break;
            }
        }

        if(j == pattern.size)
        {
            return(const char *)(sv.data);
        }
    }

    return 0;
}

StringView sv_find_by_delim
(
    StringView sv,
    char       delim,
    uint32_t   index
){
    StringView result        = {0};
    uint32_t   delim_count   = 0;
    const char *lastword_end = 0;

    if(sv.data == 0)
    {
        return result;
    }

    if(index == 0)
    {
        result.data = sv.data;
        uint32_t i  = 0;

        for(; result.data < (sv.data + sv.size) && sv.data[i] == delim; ++i)
        {
            ++result.data;
        }
        for(; result.size < sv.size && sv.data[i] != delim; ++i)
        {
            ++result.size;
        }

        return result;
    }

    for(uint64_t i = 0; i < sv.size; ++i)
    {
        if(sv.data[i] == delim)
        {
            ++delim_count;

            for(; sv.data[i] == delim; ++i)
            {
            }

            lastword_end = sv.data + i;
            printf("identified end of delim @ sv.data + %lu\n", i);
        }

        // TODO: index 1 needs to return "hello"
        // ;aaa;;hello;test;;123

        if(delim_count == index)
        {
        }
    }

    return (StringView){0};
}

const char *sv_concat
(
    StringView first,
    StringView second
){
    if(!first.data || !second.data)
    {
        fprintf(stderr, "\033[31mERROR: bad stringview data pointer.\033[0m\n");
        return 0;
    }

    const char *first_data  = sv_cstr(first);
    const char *second_data = sv_cstr(second);

    char *result = malloc(first.size + second.size + 1);
    memcpy((void*)result, (void*)first_data, first.size);
    memcpy((void*)(result + first.size), (void*)second_data, second.size);
    result[first.size + second.size] = '\0';

    free((void*)first_data);
    free((void*)second_data);

    return result;
}

#endif
