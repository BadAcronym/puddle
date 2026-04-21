#define STRING_VIEW_IMPL
#include "string_view.h"

#include <stdio.h>

int main
(
    void
){
    StringView test        = puddle_cstr_sv("Hello, World!");
    StringView substr      = puddle_sv_substr(&test, 7, test.size);
    StringView substr_test = puddle_cstr_sv("World!");
    StringView sub_test_0  = puddle_cstr_sv("W");
    StringView sub_test_1  = puddle_sv_substr(&test, 7, 7);
    StringView test_left   = puddle_cstr_sv("lo, World!");
    StringView test_right  = puddle_cstr_sv("lo, Worl");
    StringView test_both   = puddle_cstr_sv(" W");
    StringView conv_src    = puddle_cstr_sv("The quick brown fox jumps over the lazy dog.");
    const char *conv_test  = puddle_sv_cstr(&conv_src);
    StringView conv_test2  = puddle_cstr_sv(conv_test);
    StringView concat_1    = puddle_cstr_sv("Test ");
    StringView concat_2    = puddle_cstr_sv("concat !");
    StringView concat_r    = puddle_cstr_sv("Test concat !");

    uint8_t result = puddle_sv_comp(&conv_src, &conv_test2);
    if(result != SV_SAME)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_puddle_cstr FAILED.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(conv_src),
                ARG_SV(conv_test2));
        fprintf(stderr, "\nerror code: %hhu", result);
        return result;
    }
    free((void*)conv_test);

    result = puddle_sv_comp(&substr, &substr_test);
    if(result != SV_SAME)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_substr FAILED.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(substr_test), ARG_SV(substr));
        fprintf(stderr, "\n%zu vs %zu", substr_test.size, substr.size);
        fprintf(stderr, "\nerror code: %hhu", result);
        return result;
    }

    result = puddle_sv_comp(&sub_test_0, &sub_test_1);
    if(result != SV_SAME)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_substr FAILED on a single character."
                "\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(sub_test_0),
                ARG_SV(sub_test_1));
        fprintf(stderr, "\n%zu vs %zu", sub_test_0.size, sub_test_1.size);
        fprintf(stderr, "\nerror code: %hhu", result);
        return result;
    }

    puddle_sv_trim(&test, 3, SV_LEFT);
    result = puddle_sv_comp(&test, &test_left);
    if(result != SV_SAME)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_trim FAILED unit test with SV_LEFT."
                "\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(test_left),
                ARG_SV(test));
        fprintf(stderr, "\nerror code: %hhu", result);
        return result;
    }

    puddle_sv_trim(&test, 2, SV_RIGHT);
    result = puddle_sv_comp(&test, &test_right);
    if(result != SV_SAME)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_trim FAILED unit test with SV_RIGHT."
                "\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(test_right),
                ARG_SV(test));
        fprintf(stderr, "\nerror code: %hhu", result);
        return result;
    }

    puddle_sv_trim(&test, 3, SV_BOTH);
    result = puddle_sv_comp(&test, &test_both);
    if(result != SV_SAME)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_trim FAILED unit test with SV_BOTH."
                "\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(test_both),
                ARG_SV(test));
        fprintf(stderr, "\nerror code: %hhu", result);
        return result;
    }

    const char *concatenated = puddle_sv_concat(&concat_1, &concat_2);
    StringView test_concat = puddle_cstr_sv(concatenated);
    result = puddle_sv_comp(&concat_r, &test_concat);
    if(result != SV_SAME)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_concat FAILED unit test with SV_BOTH."
                "\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(concat_r),
                ARG_SV(test_concat));
        fprintf(stderr, "\nerror code: %hhu", result);
        return result;
    }
    free((void*)concatenated);

    printf("\033[32;1;1m\nSUCCESS: all unit tests passed.\033[0m\n");
    return 0;
}
