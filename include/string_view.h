#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SV_LEFT  0L
#define SV_RIGHT 1L
#define SV_BOTH  2L

#define SV_GREATER 0L
#define SV_LESSER  1L

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

// will create a stringview.
// requires a buffer to be passed that can fit the contents.
StringView cstr_sv_cpy
(
    const char *cstr,
    char       *buf
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

// will create a copied StringView from a String. just like sv_cstr.
StringView str_sv_cpy
(
    String str,
    char   *buf
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

// will return `SV_LESSER` (1) if the first sv is alphabetically lesser to the
// second, `SV_GREATER` (0) if it isn't.
uint8_t sv_is_lesser
(
    StringView first,
    StringView second
);

// Will sort the given StringView in alphabetical order, respecting the delimiter
// given: `"hello;test;path;123"` -> `"123;hello;path;test"`.
// Will return the resulting, sorted string into `buf`, which needs to be at least as
// big as `sv.size + 1`.
void sv_sort_by_delim
(
    StringView sv,
    char       delim,
    char       *buf
);

// concatenates `first` and `second` one after the other.
// `first`  + `second` = `result`.
// "Hello " + "World"  = "Hello World".
// will write into buf.
// the `buf` pointer needs to have enough space for `first.size + second.size + 1`.
void sv_concat
(
    StringView first,
    StringView second,
    char       *buf
);
#endif
