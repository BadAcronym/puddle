#include "string_view.h"

#include <stdio.h>

int main
(
    void
){
    StringView test        = cstr_sv("Hello, World!");
    StringView substr      = sv_substr(&test, 7, test.size);
    StringView substr_test = cstr_sv("World!");
    StringView test_left   = cstr_sv("lo, World!");
    StringView test_right  = cstr_sv("lo, Worl");
    StringView test_both   = cstr_sv(" W");

    uint8_t result = sv_comp(&substr, &substr_test);
    if(result != SV_SAME)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_substr FAILED.\033[0m\n");
        fprintf(stderr, "expected: " PRI_SV "\ngot: " PRI_SV, ARG_SV(substr_test), ARG_SV(substr));
        return result;
    }

    sv_trim(&test, 3, SV_LEFT);
    result = sv_comp(&test, &test_left);
    if(result != SV_SAME)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_trim FAILED unit test with SV_LEFT.\033[0m\n");
        return result;
    }

    sv_trim(&test, 2, SV_RIGHT);
    result = sv_comp(&test, &test_right);
    if(result != SV_SAME)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_trim FAILED unit test with SV_RIGHT.\033[0m\n");
        return result;
    }

    sv_trim(&test, 3, SV_BOTH);
    result = sv_comp(&test, &test_both);
    if(result != SV_SAME)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_trim FAILED unit test with SV_BOTH.\033[0m\n");
        return result;
    }

    printf("\n");
    return 0;
}
