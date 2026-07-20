#include <stdio.h>

#include "string_view.h"

int main
(
    void
){
    StringView test        = cstr_sv("Hello, World!");
    StringView substr      = sv_substr(&test, 7, test.size);
    StringView substr_test = cstr_sv("World!");
    StringView sub_test_0  = cstr_sv("W");
    StringView sub_test_1  = sv_substr(&test, 7, 7);
    StringView test_left   = cstr_sv("lo, World!");
    StringView test_right  = cstr_sv("lo, Worl");
    StringView test_both   = cstr_sv(" W");
    StringView conv_src    = cstr_sv("The quick lilac fox jumps over the dog.");
    char conv_test[conv_src.size + 1];
    sv_cstr(conv_src, conv_test);
    StringView conv_test2  = cstr_sv(conv_test);
    StringView concat_1    = cstr_sv("Test ");
    StringView concat_2    = cstr_sv("concat !");
    StringView concat_r    = cstr_sv("Test concat !");

    int num_failed = 0;

    if(!sv_same(conv_src, conv_test2))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_cstr FAILED.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(conv_src),
                ARG_SV(conv_test2));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_cstr unit test.\033[0m\n");
    }

    if(!sv_same(substr, substr_test))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_substr FAILED.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(substr_test), ARG_SV(substr));
        fprintf(stderr, "\n%zu vs %zu", substr_test.size, substr.size);
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_substr unit test.\033[0m\n");
    }

    if(!sv_same(sub_test_0, sub_test_1))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_substr FAILED "
                "on a single character.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(sub_test_0),
                ARG_SV(sub_test_1));
        fprintf(stderr, "\n%zu vs %zu", sub_test_0.size, sub_test_1.size);
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_substr on a single character."
                "\033[0m\n");
    }

    sv_trim(&test, 3, SV_LEFT);
    if(!sv_same(test, test_left))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_trim FAILED unit test "
                "with SV_LEFT.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(test_left),
                ARG_SV(test));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_trim unit test with SV_LEFT."
                "\033[0m\n");
    }

    sv_trim(&test, 2, SV_RIGHT);
    if(!sv_same(test, test_right))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_trim FAILED unit test "
                "with SV_RIGHT.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(test_right),
                ARG_SV(test));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_trim unit test with SV_RIGHT."
                "\033[0m\n");
    }

    sv_trim(&test, 3, SV_BOTH);
    if(!sv_same(test, test_both))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_trim FAILED unit test "
                "with SV_BOTH.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(test_both),
                ARG_SV(test));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_trim unit test with SV_BOTH."
                "\033[0m\n");
    }

    char concatenated[concat_1.size + concat_2.size + 1];
    sv_concat(concat_1, concat_2, concatenated);
    StringView test_concat = cstr_sv(concatenated);
    if(!sv_same(concat_r, test_concat))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_concat FAILED unit test.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"", ARG_SV(concat_r),
                ARG_SV(test_concat));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_concat unit test.\033[0m\n");
    }

    StringView bigStr = cstr_sv("The quick brown fox jumps over the lazy dog.");
    StringView small0 = cstr_sv("The ");
    StringView small1 = cstr_sv(" The");
    StringView small2 = cstr_sv(" jumps ");
    StringView small3 = cstr_sv("dog.");
    StringView small4 = cstr_sv(".");

    if(sv_is_substr(small1, bigStr))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_is_substr FAILED unit test "
                "with SV_DIFFERENT.\033[0m\n");
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_is_substr unit test with "
                "SV_DIFFERENT.\033[0m\n");
    }

    if(!sv_is_substr(small0, bigStr))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_is_substr FAILED unit test "
                "with SV_IS_SUBSTR no 1.\033[0m\n");
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_is_substr unit test with "
                "SV_IS_SUBSTR no 1.\033[0m\n");
    }

    if(!sv_is_substr(small2, bigStr))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_is_substr FAILED unit test "
                "with SV_IS_SUBSTR no 2.\033[0m\n");
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_is_substr unit test with "
                "SV_IS_SUBSTR no 2.\033[0m\n");
    }

    if(!sv_is_substr(small3, bigStr))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_is_substr FAILED unit test "
                "with SV_IS_SUBSTR no 3.\033[0m\n");
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_is_substr unit test with "
                "SV_IS_SUBSTR no 3.\033[0m\n");
    }

    if(sv_find(small1, bigStr))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find FAILED unit test with "
                "non-findable pattern.\033[0m\n");
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find unit test with "
                "non-findable pattern.\033[0m\n");
    }

    const char *found = sv_find(small0, bigStr);
    if(!found)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find FAILED to find substring."
                "\033[0m\n");
        ++num_failed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small0.size)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find FAILED by returning an "
                "invalid pointer, no 1. \033[0m\n");
        fprintf(stderr, "StringView sv valid range: %p - %p.\nReturned pointer was: %p."
                "\nThat's %li away from the start and %li away from the end.\n",
                bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                bigStr.data + bigStr.size - found);
        ++num_failed;
    }
    else if(found != bigStr.data)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find FAILED by returning the wrong "
                "offset. Expected 0, got: %u\033[0m\n",
                (uint32_t)(found - bigStr.data));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find unit test by returning "
                "a valid pointer, no 1.\033[0m\n");
    }

    found = sv_find(small2, bigStr);
    if(!found)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find FAILED to find substring."
                "\033[0m\n");
        ++num_failed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small2.size)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find FAILED by returning an "
                "invalid pointer, no 2. \033[0m\n");
        fprintf(stderr, "StringView sv valid range: %p - %p.\nReturned pointer was: %p."
                "\nThat's %li away from the start and %li away from the end.\n",
                bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                bigStr.data + bigStr.size - found);
        ++num_failed;
    }
    else if(found != bigStr.data + 19)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find FAILED by returning the wrong "
                "offset. Expected 20, got: %u\033[0m\n",
                (uint32_t)(found - bigStr.data));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find unit test by returning "
                "a valid pointer, no 2.\033[0m\n");
    }

    found = sv_find(small3, bigStr);
    if(!found)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find FAILED to find substring."
                "\033[0m\n");
        ++num_failed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small3.size)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find FAILED by returning an "
                "invalid pointer, no 3. \033[0m\n");
        fprintf(stderr, "StringView sv valid range: %p - %p.\nReturned pointer was: %p."
                "\nThat's %li away from the start and %li away from the end.\n",
                bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                bigStr.data + bigStr.size - found);
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find unit test by returning "
                "a valid pointer, no 3.\033[0m\n");
    }

    found = sv_find(small4, bigStr);
    if(!found)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find FAILED to find substring."
                "\033[0m\n");
        ++num_failed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small4.size)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find FAILED by returning an "
                "invalid pointer, no 4. \033[0m\n");
        fprintf(stderr, "StringView sv valid range: %p - %p.\nReturned pointer was: %p."
                "\nThat's %li away from the start and %li away from the end.\n",
                bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                bigStr.data + bigStr.size - found);
        ++num_failed;
    }
    else if(found != bigStr.data + 43)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find FAILED by returning the wrong "
                "offset. Expected 43, got: %u\033[0m\n",
                (uint32_t)(found - bigStr.data));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find unit test by returning "
                "a valid pointer, no 4.\033[0m\n");
    }

    StringView testPath = cstr_sv(";zeroeth;;first;second;third;;;;;;;;fourth;");

    StringView testFile0 = sv_find_by_delim(testPath, ';', 0);
    StringView expected0 = cstr_sv("zeroeth");
    if(!sv_same(testFile0, expected0))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find_by_delim FAILED unit test "
                "file0.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(expected0), ARG_SV(testFile0));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find_by_delim unit test with "
                "index = 0.\033[0m\n");
    }

    StringView testFile1 = sv_find_by_delim(testPath, ';', 1);
    StringView expected1 = cstr_sv("first");
    if(!sv_same(testFile1, expected1))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find_by_delim FAILED unit test "
                "file1.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n", ARG_SV(expected1),
                ARG_SV(testFile1));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find_by_delim unit test with "
                "index = 1.\033[0m\n");
    }

    StringView testFile2 = sv_find_by_delim(testPath, ';', 2);
    StringView expected2 = cstr_sv("second");
    if(!sv_same(testFile2, expected2))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find_by_delim FAILED unit test "
                "file2.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n", ARG_SV(expected2),
                ARG_SV(testFile2));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find_by_delim unit test with "
                "index = 2.\033[0m\n");
    }

    StringView testFile3 = sv_find_by_delim(testPath, ';', 3);
    StringView expected3 = cstr_sv("third");
    if(!sv_same(testFile3, expected3))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find_by_delim FAILED unit test "
                "file3.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n", ARG_SV(expected3),
                ARG_SV(testFile3));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find_by_delim unit test with "
                "index = 3.\033[0m\n");
    }

    StringView testFile4 = sv_find_by_delim(testPath, ';', 4);
    StringView expected4 = cstr_sv("fourth");
    if(!sv_same(testFile4, expected4))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find_by_delim FAILED unit test "
                "file4.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n", ARG_SV(expected4),
                ARG_SV(testFile4));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find_by_delim unit test with "
                "index = 4.\033[0m\n");
    }

    StringView testPath2 = cstr_sv("zero:one:two::::");

    StringView testFile2_0 = sv_find_by_delim(testPath2, ':', 0);
    StringView expected2_0 = cstr_sv("zero");
    if(!sv_same(testFile2_0, expected2_0))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find_by_delim FAILED unit test "
                "file0 in path 2.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(expected2_0), ARG_SV(testFile2_0));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find_by_delim unit test with "
                "index = 2_0.\033[0m\n");
    }

    StringView testFile2_1 = sv_find_by_delim(testPath2, ':', 1);
    StringView expected2_1 = cstr_sv("one");
    if(!sv_same(testFile2_1, expected2_1))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find_by_delim FAILED unit test "
                "file1 in path 2.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(expected2_1), ARG_SV(testFile2_1));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find_by_delim unit test with "
                "index = 2_1.\033[0m\n");
    }

    StringView testFile2_2 = sv_find_by_delim(testPath2, ':', 2);
    StringView expected2_2 = cstr_sv("two");
    if(!sv_same(testFile2_2, expected2_2))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_find_by_delim FAILED unit test "
                "file2 in path 2.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(expected2_2), ARG_SV(testFile2_2));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_find_by_delim unit test with "
                "index = 2_2.\033[0m\n");
    }

    uint32_t result = sv_count_by_delim(testPath, ';');
    if(result != 5)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_count_by_delim FAILED unit test "
                "counting symbols in testPath.\033[0m\n");
        fprintf(stderr, "expected: 5\ngot: %u\n", result);
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_count_by_delim unit test with "
                "testPath.\033[0m\n");
    }

    result = sv_count_by_delim(testPath2, ':');
    if(result != 3)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_count_by_delim FAILED unit test "
                "counting symbols in testPath2.\033[0m\n");
        fprintf(stderr, "expected: 3\ngot: %u\n", result);
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_count_by_delim unit test with "
                "testPath2.\033[0m\n");
    }

    StringView testPath_tosort = cstr_sv("abc:abd:123:aaa");
    char sorted[testPath_tosort.size + 1];
    sv_sort_by_delim(testPath_tosort, ':', sorted);

    StringView testPath_sorted =
    {
        .data = sorted,
        .size = testPath_tosort.size
    };

    StringView a = cstr_sv("..");
    StringView b = cstr_sv(".");
    if(sv_is_lesser(a, b))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_is_lesser FAILED unit test "
                "with \"..\" vs \".\".\033[0m\n");
        fprintf(stderr, "expected: 0\ngot: 1\n");
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_is_lesser unit test "
                "with \"..\" vs \".\".\033[0m\n");
    }

    StringView a1 = cstr_sv("123");
    StringView b1 = cstr_sv("ab");
    if(sv_is_lesser(b1, a1))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_is_lesser FAILED unit test "
                "with \"123\" vs \"abc\".\033[0m\n");
        fprintf(stderr, "expected: 0\ngot: 1\n");
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_is_lesser unit test "
                "with \"123\" vs \"abc\".\033[0m\n");
    }

    result = sv_count_by_delim(testPath_sorted, ':');
    if(result != 4)
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_count_by_delim FAILED unit test "
                "counting symbols in testPath_sorted.\033[0m\n");
        fprintf(stderr, "expected: 4\ngot: %u\n", result);
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_count_by_delim unit test with "
                "testPath_sorted.\033[0m\n");
    }

    StringView testPath_real = cstr_sv("32 32 textures 2.qoi;"
                                       ".;32 32 textures master file.aseprite;"
                                       "water edge texture 1 32 32.qoi;"
                                       "grass 32 32 animated.qoi;"
                                       "water edge texture 2 32 32.qoi;"
                                       "32 32 textures master.qoi;"
                                       "dirt texture 1 32 32.qoi;"
                                       "grass 32 32 1.qoi;"
                                       "dirt texture 2 32 32.qoi;"
                                       "water edge texture 3 32 32.qoi;..;"
                                       "animated shallow water.qoi;"
                                       "water edge texture 4 32 32.qoi;");
    char sorted_real[testPath_real.size + 1];
    sv_sort_by_delim(testPath_real, ';', sorted_real);
    testPath_real.data = sorted_real;

    StringView single_dot = cstr_sv(".");
    StringView two_dot    = cstr_sv("..");
    StringView one_test;
    StringView two_test;
    one_test = sv_find_by_delim(testPath_real, ';', 0);
    two_test = sv_find_by_delim(testPath_real, ';', 1);

    if(!sv_same(one_test, single_dot))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_sort_by_delim FAILED unit test "
                "with real path and the '.' identifier.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(single_dot), ARG_SV(one_test));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_sort_by_delim unit test with "
                "real path and the '.' identifier.\033[0m\n");
    }

    if(!sv_same(two_test, two_dot))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_sort_by_delim FAILED unit test "
                "with real path and the '..' identifier.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(two_dot), ARG_SV(two_test));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_sort_by_delim unit test with "
                "real path and the '..' identifier.\033[0m\n");
    }

    StringView expected_sort0 = cstr_sv("123");
    StringView testFile_sort0 = sv_find_by_delim(testPath_sorted, ':', 0);
    if(!sv_same(testFile_sort0, expected_sort0))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_sort_by_delim FAILED unit test "
                "with index 0.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(expected_sort0), ARG_SV(testFile_sort0));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_sort_by_delim unit test with "
                "index = 0.\033[0m\n");
    }

    StringView expected_sort1 = cstr_sv("aaa");
    StringView testFile_sort1 = sv_find_by_delim(testPath_sorted, ':', 1);
    if(!sv_same(testFile_sort1, expected_sort1))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_sort_by_delim FAILED unit test "
                "with index 1.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(expected_sort1), ARG_SV(testFile_sort1));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_sort_by_delim unit test with "
                "index = 1.\033[0m\n");
    }

    StringView expected_sort2 = cstr_sv("abc");
    StringView testFile_sort2 = sv_find_by_delim(testPath_sorted, ':', 2);
    if(!sv_same(testFile_sort2, expected_sort2))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_sort_by_delim FAILED unit test "
                "with index 2.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(expected_sort2), ARG_SV(testFile_sort2));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_sort_by_delim unit test with "
                "index = 2.\033[0m\n");
    }

    StringView expected_sort3 = cstr_sv("abd");
    StringView testFile_sort3 = sv_find_by_delim(testPath_sorted, ':', 3);
    if(!sv_same(testFile_sort2, expected_sort2))
    {
        fprintf(stderr, "\033[31;1;1mERROR: sv_sort_by_delim FAILED unit test "
                "with index 3.\033[0m\n");
        fprintf(stderr, "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(expected_sort3), ARG_SV(testFile_sort3));
        ++num_failed;
    }
    else
    {
        fprintf(stderr, "\033[32;1;1mSUCCESS: passed sv_sort_by_delim unit test with "
                "index = 3.\033[0m\n");
    }

    if(!num_failed)
    {
        printf("\033[32;1;1m\nSUCCESS: all unit tests passed.\033[0m\n");
    }
    else
    {
        printf("\033[31;1;1m\nERROR: failed %i unit tests.\033[0m\n", num_failed);
    }

    return num_failed;
}
