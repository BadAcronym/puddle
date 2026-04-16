#include <stddef.h>
#include <stdint.h>

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
extern StringView cstr_sv
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

// will trim count characters from: SV_LEFT, SV_RIGHT or SV_BOTH.
// in the case of both, it will first trim count from the left,
// then try to trim count from the right.
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
// 1: `SV_LONGER_SECOND`, analogous to `SV_LONGER_FIRST`, but vice-versa.
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
