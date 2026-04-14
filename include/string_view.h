#include <stddef.h>
#include <stdint.h>

// sv_trim directions
#define SV_LEFT  0L
#define SV_RIGHT 1L
#define SV_BOTH  2L

// sv_comp results
#define SV_SAME           0L
#define SV_ISSUBSTR_LEFT  1L
#define SV_ISSUBSTR_RIGHT 2L
#define SV_DIFFERENT      3L

// macros for printf() formatting and printing.
// TODO: #define

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
extern const char *sv_cstr
(
    const char *sv
);

// will return the trimmed substring as a new stringview.
// start_pos & end_pos are both positions in the original string.
extern StringView sv_substr
(
    size_t start_pos,
    size_t end_pos
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
// SV_SAME if the stringviews have the same content.
// SV_ISSUBSTR_FIRST, if the first sv is contained fully in the right sv, but the right sv is larger.
// SV_ISSUBSTR_SECOND, analogous to -FIRST, but vice-versa.
// SV_DIFFERENT, if they are different stringviews.
extern uint8_t sv_comp
(
    StringView *first,
    StringView *second
);
