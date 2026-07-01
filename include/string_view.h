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
String cstr_str
(
    char *cstr
);

// will create a string, but will not just pass a pointer. Will actually
// copy its contents and allocate to a new pointer.
String cstr_str_cpy
(
    char *cstr
);

// will create a stringview from a cstring, by passing the pointer given
// as its data and counting its length.
StringView cstr_sv
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
// will use the provided buffer pointer to write the data into.
// The buffer needs to be at least `sv.size + 1` big.
void sv_cstr
(
    StringView sv,
    char       *buf
);

// just like sv_cstr, but for strings :)
void str_cstr
(
    String str,
    char   *buf
);

// will create a copied StringView from a string. Its data will be newly allocated.
StringView str_sv_cpy
(
    String str
);

// will return the trimmed substring as a new stringview.
// start_pos & end_pos are both positions in the original string.
StringView sv_substr
(
    StringView *sv,
    size_t     start_pos,
    size_t     end_pos
);

// will trim count characters from: `SV_LEFT`, `SV_RIGHT` or `SV_BOTH`.
// in the case of both, it will first trim `count` from the left,
// then try to trim `count` from the right.
void sv_trim
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
uint8_t sv_same
(
    StringView first,
    StringView second
);

// Will return:
// 0: `SV_DIFFERENT`, if the `first` string is `not` included in the `second` string.
// 1: `SV_SAME`, if the stringviews have the same content and are the same length.
// 2: `SV_IS_SUBSTR`, if the `first` string is `fully` included in the `second` string.
uint8_t sv_is_substr
(
    StringView first,
    StringView second
);

// Will return a pointer to the start of `pattern` inside `sv`, if it was found.
// If it wasn't found, the pointer is null.
const char *sv_find
(
    StringView pattern,
    StringView sv
);

// Will return a StringView that finds the `i-th` element in `sv`, where an element is
// a substring that starts with either the start of the string or a `delim` character
// and ends with either the end of the string or a `delim` character. E.g., in the
// string: `"hello;test;path;for;example"`, and `delim = ';'`, `i = 0` would yield
// "hello", `i = 3` yields "for", etc.
//
// ignores duplicate delimiters, i.e. "hello;test;path;for;example" and
// ";;hello;test;path;;;;;for;example;;" will yield the same results.
//
// if no more results can be found (the index is too high), a null StringView will be
// returned.
StringView sv_find_by_delim
(
    StringView sv,
    char       delim,
    uint32_t   index
);

// Will count the amount of substrings that are separated by the given delimiter.
uint32_t sv_count_by_delim
(
    StringView sv,
    char       delim
);

// will return 1 if the first sv is alphabetically lesser to the second, 0 if it isn't.
uint8_t sv_is_lesser
(
    StringView first,
    StringView second
);

// Will sort the given StringView in alphabetical order, respecting the delimiter
// given: `"hello;test;path;123"` -> `"123;hello;path;test"`.
// Returns a newly allocated c string, which can be absorbed into a StringView with the
// same size as the input sv.
const char *sv_sort_by_delim
(
    StringView sv,
    char       delim
);

// concatenates `first` and `second` one after the other.
// `first`  + `second` = `result`.
// "Hello " + "World"  = "Hello World".
const char *sv_concat
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

String cstr_str_cpy
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

void sv_cstr
(
    StringView sv,
    char       *buf
){
    if(!buf)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as buf.\033[0m\n");
        return;
    }

    memcpy(buf, sv.data, sv.size);
    buf[sv.size] = '\0';
}

void str_cstr
(
    String str,
    char   *buf
){
    if(!buf)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as buf.\033[0m\n");
        return;
    }

    memcpy(buf, str.data, str.size);
    buf[str.size] = '\0';
}

StringView str_sv_cpy
(
    String str
){
    char *buf = malloc(str.size);

    for(uint32_t i = 0; i < str.size; ++i)
    {
        buf[i] = str.data[i];
    }

    return(StringView)
    {
        .data = buf,
        .size = str.size
    };
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
    if((first.size == second.size && first.data == second.data) ||
       (!first.size && !second.size)
    ){
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
            return(&sv.data[i]);
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
    StringView result          = {0};
    uint32_t   delim_count     = 0;
    const char *nextword_start = sv.data;

    if(sv.data == 0)
    {
        return result;
    }

    if(sv.data[0] == delim)
    {
        ++index;
    }

    for(uint64_t i = 0; i < sv.size; ++i)
    {
        size_t substr_size = 0;

        if(sv.data[i] == delim)
        {
            ++delim_count;

            for(; i < sv.size && sv.data[i] == delim; ++i)
            {
            }

            nextword_start = sv.data + i;
        }

        if(delim_count == index)
        {
            for(; i < sv.size && sv.data[i] != delim; ++i)
            {
                ++substr_size;
            }

            return(StringView)
            {
                .data = nextword_start,
                .size = substr_size
            };
        }
    }

    return (StringView){0};
}

uint32_t sv_count_by_delim
(
    StringView sv,
    char       delim
){
    uint32_t delim_count = 0;

    if(sv.data == 0)
    {
        return 0;
    }

    for(uint32_t i = 0; i < sv.size; ++i)
    {
        if(sv.data[i] == delim)
        {
            ++delim_count;

            for(; i < sv.size && sv.data[i] == delim; ++i)
            {
            }
        }
    }

    if(sv.data[sv.size - 1] != delim && sv.data[0] != delim)
    {
        return delim_count + 1;
    }
    else if(sv.data[sv.size - 1] == delim && sv.data[0] == delim)
    {
        return delim_count - 1;
    }

    return delim_count;
}

uint8_t sv_is_lesser
(
    StringView first,
    StringView second
){
    for(uint32_t i = 0; i < first.size && second.size; ++i)
    {
        if(first.data[i] < second.data[i])
        {
            return 1;
        }
        else if(second.data[i] < first.data[i])
        {
            return 0;
        }

        if(i == first.size - 1 && first.size < second.size)
        {
            return 1;
        }
        else if(i == second.size - 1 && second.size < first.size)
        {
            return 0;
        }
    }

    return 0;
}

const char *sv_sort_by_delim
(
    StringView sv,
    char       delim
){
    uint32_t   count = sv_count_by_delim(sv, delim);
    StringView *buf  = malloc(count * sizeof(StringView));

    for(uint32_t i = 0; i < count; ++i)
    {
        buf[i] = sv_find_by_delim(sv, delim, i);
    }

    for(uint32_t i = 0; i < count - 1; ++i)
    {
        uint8_t swapped = 0;
        for(uint32_t j = 0; j < count - i - 1; ++j)
        {
            if(sv_is_lesser(buf[j + 1], buf[j]))
            {
                StringView tmp = buf[j];
                buf[j] = buf[j + 1];
                buf[j + 1] = tmp;
                swapped = 1;
            }
        }

        if(!swapped)
        {
            break;
        }
    }

    char *result = malloc(sv.size + 1);

    uint32_t offset = 0;
    for(uint32_t i = 0; i < count; ++i)
    {
        for(uint32_t j = 0; j < buf[i].size; ++j)
        {
            result[offset + j] = buf[i].data[j];
        }

        if(offset + buf[i].size < sv.size)
        {
            result[offset + buf[i].size] = delim;
        }
        offset += buf[i].size + 1;
    }

    result[sv.size] = '\0';

    free(buf);
    return result;
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

    char first_data[first.size + 1];
    char second_data[second.size + 1];

    sv_cstr(first, first_data);
    sv_cstr(second, second_data);

    char *result = malloc(first.size + second.size + 1);
    memcpy((void*)result, (void*)first_data, first.size);
    memcpy((void*)(result + first.size), (void*)second_data, second.size);
    result[first.size + second.size] = '\0';

    return result;
}

#endif
