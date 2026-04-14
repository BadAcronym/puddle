#include <stddef.h>
#include <stdint.h>

#define SV_LEFT  0L
#define SV_RIGHT 1L
#define SV_BOTH  2L

// length-based string.
// data is not guaranteed to be null-terminated.
// use sv_cstr() if you require access to a cstring.
typedef struct sv
{
    const char *data;
    size_t     size;
}
StringView;

// will return the trimmed substring as a new stringview.
// start_pos & end_pos are both positions in the original string.
extern StringView sv_substr
(
    size_t start_pos,
    size_t end_pos
);

// will trim count from: SV_LEFT, SV_RIGHT or SV_BOTH.
// in the case of both, it will first trim count from the left,
// then try to trim count from the right.
extern void sv_trim
(
    StringView *sv,
    size_t     count,
    uint8_t    direction
);

// will create a stringview from a cstring.
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
