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
    StringView conv_src    = puddle_cstr_sv("The quick lilac fox jumps over the dog.");
    const char *conv_test  = puddle_sv_cstr(conv_src);
    StringView conv_test2  = puddle_cstr_sv(conv_test);
    StringView concat_1    = puddle_cstr_sv("Test ");
    StringView concat_2    = puddle_cstr_sv("concat !");
    StringView concat_r    = puddle_cstr_sv("Test concat !");

    if(!puddle_sv_same(conv_src, conv_test2))
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_cstr FAILED.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(conv_src),
                ARG_SV(conv_test2));
        return 1;
    }
    free((void*)conv_test);

    if(!puddle_sv_same(substr, substr_test))
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_substr FAILED.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(substr_test), ARG_SV(substr));
        fprintf(stderr, "\n%zu vs %zu", substr_test.size, substr.size);
        return 2;
    }

    if(!puddle_sv_same(sub_test_0, sub_test_1))
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_substr FAILED "
                "on a single character.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(sub_test_0),
                ARG_SV(sub_test_1));
        fprintf(stderr, "\n%zu vs %zu", sub_test_0.size, sub_test_1.size);
        return 3;
    }

    puddle_sv_trim(&test, 3, SV_LEFT);
    if(!puddle_sv_same(test, test_left))
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_trim FAILED unit test "
                "with SV_LEFT.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(test_left),
                ARG_SV(test));
        return 4;
    }

    puddle_sv_trim(&test, 2, SV_RIGHT);
    if(!puddle_sv_same(test, test_right))
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_trim FAILED unit test "
                "with SV_RIGHT.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(test_right),
                ARG_SV(test));
        return 5;
    }

    puddle_sv_trim(&test, 3, SV_BOTH);
    if(!puddle_sv_same(test, test_both))
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_trim FAILED unit test "
                "with SV_BOTH.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(test_both),
                ARG_SV(test));
        return 6;
    }

    const char *concatenated = puddle_sv_concat(concat_1, concat_2);
    StringView test_concat   = puddle_cstr_sv(concatenated);
    if(!puddle_sv_same(concat_r, test_concat))
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_concat FAILED unit test "
                "with SV_BOTH.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(concat_r),
                ARG_SV(test_concat));
        return 7;
    }
    free((void*)concatenated);

    StringView bigStr = puddle_cstr_sv("The quick brown fox jumps over the lazy dog.");
    StringView small0 = puddle_cstr_sv(" The ");
    StringView small1 = puddle_cstr_sv("The");
    StringView small2 = puddle_cstr_sv(" jumps ");
    StringView small3 = puddle_cstr_sv("dog.");

    if(puddle_sv_is_substr(small0, bigStr))
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_is_substr FAILED unit test "
                "with SV_DIFFERENT.\033[0m\n");
        return 8;
    }
    if(!puddle_sv_is_substr(small1, bigStr))
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_is_substr FAILED unit test "
                "with SV_IS_SUBSTR no 1.\033[0m\n");
        return 9;
    }
    if(!puddle_sv_is_substr(small2, bigStr))
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_is_substr FAILED unit test "
                "with SV_IS_SUBSTR no 2.\033[0m\n");
        return 10;
    }
    if(!puddle_sv_is_substr(small3, bigStr))
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_is_substr FAILED unit test "
                "with SV_IS_SUBSTR no 3.\033[0m\n");
        return 11;
    }

    if(puddle_sv_find(small0, bigStr))
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_find FAILED unit test with "
                "non-findable pattern.\033[0m\n");
        return 12;
    }

    const char *found = puddle_sv_find(small1, bigStr);
    if(!found)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_find FAILED to find substring."
                "\033[0m\n");
        return 13;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small1.size)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_find FAILED by returning an "
                "invalid pointer. \033[0m\n");
        fprintf(stderr, "StringView sv valid range: %p - %p.\nReturned pointer was: %p."
                "\nThat's %li away from the start and %li away from the end.",
                bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                bigStr.data + bigStr.size - found);
        return 14;
    }

    found = puddle_sv_find(small2, bigStr);
    if(!found)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_find FAILED to find substring."
                "\033[0m\n");
        return 15;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small2.size)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_find FAILED by returning an "
                "invalid pointer. \033[0m\n");
        fprintf(stderr, "StringView sv valid range: %p - %p.\nReturned pointer was: %p."
                "\nThat's %li away from the start and %li away from the end.",
                bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                bigStr.data + bigStr.size - found);
        return 16;
    }

    found = puddle_sv_find(small3, bigStr);
    if(!found)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_find FAILED to find substring."
                "\033[0m\n");
        return 17;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small3.size)
    {
        fprintf(stderr, "\033[31;1;1mERROR: puddle_sv_find FAILED by returning an "
                "invalid pointer. \033[0m\n");
        fprintf(stderr, "StringView sv valid range: %p - %p.\nReturned pointer was: %p."
                "\nThat's %li away from the start and %li away from the end.",
                bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                bigStr.data + bigStr.size - found);
        return 18;
    }

    printf("\033[32;1;1m\nSUCCESS: all unit tests passed.\033[0m\n");
    return 0;
}
