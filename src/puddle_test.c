#include "string_view.h"
#include "dynamic_array.h"
#include "pd_path.h"
#include "pd_print_macros.h"

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
        PD_FAIL("sv_cstr. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                 ARG_SV(conv_src), ARG_SV(conv_test2));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_cstr.");
    }

    if(!sv_same(substr, substr_test))
    {
        PD_FAIL("sv_substr. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(substr_test), ARG_SV(substr));
        PD_DEBUG("%zu vs %zu", substr_test.size, substr.size);
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_substr.");
    }

    if(!sv_same(sub_test_0, sub_test_1))
    {
        PD_FAIL("sv_substr on a character. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(sub_test_0), ARG_SV(sub_test_1));
        PD_DEBUG("%zu vs %zu", substr_test.size, substr.size);
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_substr on a character.");
    }

    sv_trim(&test, 3, SV_LEFT);
    if(!sv_same(test, test_left))
    {
        PD_FAIL("sv_trim with SV_LEFT. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(test_left), ARG_SV(test));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_trim with SV_LEFT.");
    }

    sv_trim(&test, 2, SV_RIGHT);
    if(!sv_same(test, test_right))
    {
        PD_FAIL("sv_trim with SV_RIGHT. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(test_right), ARG_SV(test));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_trim with SV_RIGHT.");
    }

    sv_trim(&test, 3, SV_BOTH);
    if(!sv_same(test, test_both))
    {
        PD_FAIL("sv_trim with SV_BOTH. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(test_both), ARG_SV(test));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_trim with SV_BOTH.");
    }

    char concatenated[concat_1.size + concat_2.size + 1];
    sv_concat(concat_1, concat_2, concatenated);
    StringView test_concat = cstr_sv(concatenated);
    if(!sv_same(concat_r, test_concat))
    {
        PD_FAIL("sv_concat. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(concat_r), ARG_SV(test_concat));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_concat unit test.");
    }

    StringView bigStr = cstr_sv("The quick brown fox jumps over the lazy dog.");
    StringView small0 = cstr_sv("The ");
    StringView small1 = cstr_sv(" The");
    StringView small2 = cstr_sv(" jumps ");
    StringView small3 = cstr_sv("dog.");
    StringView small4 = cstr_sv(".");

    if(sv_is_substr(small1, bigStr))
    {
        PD_FAIL("sv_is_substr with SV_DIFFERENT.");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_is_substr with SV_DIFFERENT.");
    }

    if(!sv_is_substr(small0, bigStr))
    {
        PD_FAIL("sv_is_substr with SV_IS_SUBSTR, 1.");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_is_substr with SV_IS_SUBSTR, 1.");
    }

    if(!sv_is_substr(small2, bigStr))
    {
        PD_FAIL("sv_is_substr with SV_IS_SUBSTR, 2.");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_is_substr with SV_IS_SUBSTR, 2.");
    }

    if(!sv_is_substr(small3, bigStr))
    {
        PD_FAIL("sv_is_substr with SV_IS_SUBSTR, 3.");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_is_substr with SV_IS_SUBSTR, 3.");
    }

    if(sv_find(small1, bigStr))
    {
        PD_FAIL("sv_find with non-findable pattern.");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find with non-findable pattern.");
    }

    const char *found = sv_find(small0, bigStr);
    if(!found)
    {
        PD_FAIL("sv_find with findable pattern, 1.");
        PD_ERROR("failed to find pattern entirely.");
        ++num_failed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small0.size)
    {
        PD_FAIL("sv_find with findable pattern, 1.");
        PD_ERROR("returned an invalid pointer. StringView sv valid range: %p - %p.\n"
                 "Returned pointer was: %p.\nThat's %li away from the start and "
                 "%li away from the end.\n",
                 bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                 bigStr.data + bigStr.size - found);
        ++num_failed;
    }
    else if(found != bigStr.data)
    {
        PD_FAIL("sv_find with findable pattern, 1.");
        PD_ERROR("returned the wrong offset. Expected: 0, got: %u\033[0m\n",
                (uint32_t)(found - bigStr.data));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find with findable pattern, 1.");
    }

    found = sv_find(small2, bigStr);
    if(!found)
    {
        PD_FAIL("sv_find with findable pattern, 2.");
        PD_ERROR("failed to find pattern entirely.");
        ++num_failed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small2.size)
    {
        PD_FAIL("sv_find with findable pattern, 2.");
        PD_ERROR("returned an invalid pointer. StringView sv valid range: %p - %p.\n"
                 "Returned pointer was: %p.\nThat's %li away from the start and "
                 "%li away from the end.\n",
                 bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                 bigStr.data + bigStr.size - found);
        ++num_failed;
    }
    else if(found != bigStr.data + 19)
    {
        PD_FAIL("sv_find with findable pattern, 2.");
        PD_ERROR("returned the wrong offset. Expected: 19, got: %u\033[0m\n",
                (uint32_t)(found - bigStr.data));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find with findable pattern, 2.");
    }

    found = sv_find(small3, bigStr);
    if(!found)
    {
        PD_FAIL("sv_find with findable pattern, 3.");
        PD_ERROR("failed to find pattern entirely.");
        ++num_failed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small3.size)
    {
        PD_FAIL("sv_find with findable pattern, 3.");
        PD_ERROR("returned an invalid pointer. StringView sv valid range: %p - %p.\n"
                 "Returned pointer was: %p.\nThat's %li away from the start and "
                 "%li away from the end.\n",
                 bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                 bigStr.data + bigStr.size - found);
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find with findable pattern, 3.");
    }

    found = sv_find(small4, bigStr);
    if(!found)
    {
        PD_FAIL("sv_find with findable pattern, 4.");
        PD_ERROR("failed to find pattern entirely.");
        ++num_failed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small4.size)
    {
        PD_FAIL("sv_find with findable pattern, 4.");
        PD_ERROR("returned an invalid pointer. StringView sv valid range: %p - %p.\n"
                 "Returned pointer was: %p.\nThat's %li away from the start and "
                 "%li away from the end.\n",
                 bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                 bigStr.data + bigStr.size - found);
        ++num_failed;
    }
    else if(found != bigStr.data + 43)
    {
        PD_FAIL("sv_find with findable pattern, 4.");
        PD_ERROR("returned the wrong offset. Expected: 43, got: %u\033[0m\n",
                (uint32_t)(found - bigStr.data));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find with findable pattern, 4.");
    }

    found = sv_find_last(small4, bigStr);
    if(found != bigStr.data + 43)
    {
        PD_FAIL("sv_find_last with findable pattern, 1.");
        PD_ERROR("returned the wrong offset. Expected: 43, got: %u\033[0m\n",
                (uint32_t)(found - bigStr.data));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_last with findable pattern, 1.");
    }

    StringView duplStr = cstr_sv("The fox jumps jumps jumps jumps");
    found = sv_find_last(small2, duplStr);
    if(found != duplStr.data + 19)
    {
        PD_FAIL("sv_find_last with findable pattern, 2.");
        PD_ERROR("returned the wrong offset. Expected: 19, got: %u\033[0m\n",
                (uint32_t)(found - duplStr.data));
    }
    else
    {
        PD_SUCCESS("passed sv_find_last with findable pattern, 2.");
    }

    StringView testPath = cstr_sv(";zeroeth;;first;second;third;;;;;;;;fourth;");

    StringView testFile0 = sv_find_by_delim(testPath, ';', 0);
    StringView expected0 = cstr_sv("zeroeth");
    if(!sv_same(testFile0, expected0))
    {
        PD_FAIL("sv_find_by_delim with index = 0. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected0), ARG_SV(testFile0));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_by_delim with index = 0.");
    }

    StringView testFile1 = sv_find_by_delim(testPath, ';', 1);
    StringView expected1 = cstr_sv("first");
    if(!sv_same(testFile1, expected1))
    {
        PD_FAIL("sv_find_by_delim with index = 1. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected1), ARG_SV(testFile1));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_by_delim with index = 1.");
    }

    StringView testFile2 = sv_find_by_delim(testPath, ';', 2);
    StringView expected2 = cstr_sv("second");
    if(!sv_same(testFile2, expected2))
    {
        PD_FAIL("sv_find_by_delim with index = 2. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected2), ARG_SV(testFile2));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_by_delim with index = 2.");
    }

    StringView testFile3 = sv_find_by_delim(testPath, ';', 3);
    StringView expected3 = cstr_sv("third");
    if(!sv_same(testFile3, expected3))
    {
        PD_FAIL("sv_find_by_delim with index = 3. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected3), ARG_SV(testFile3));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_by_delim with index = 3.");
    }

    StringView testFile4 = sv_find_by_delim(testPath, ';', 4);
    StringView expected4 = cstr_sv("fourth");
    if(!sv_same(testFile4, expected4))
    {
        PD_FAIL("sv_find_by_delim with index = 4. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected4), ARG_SV(testFile4));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_by_delim with index = 4.");
    }

    StringView testPath2 = cstr_sv("zero:one:two::::");

    StringView testFile2_0 = sv_find_by_delim(testPath2, ':', 0);
    StringView expected2_0 = cstr_sv("zero");
    if(!sv_same(testFile2_0, expected2_0))
    {
        PD_FAIL("sv_find_by_delim with index = 2_0. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected2_0), ARG_SV(testFile2_0));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_by_delim with index = 2_0.");
    }

    StringView testFile2_1 = sv_find_by_delim(testPath2, ':', 1);
    StringView expected2_1 = cstr_sv("one");
    if(!sv_same(testFile2_1, expected2_1))
    {
        PD_FAIL("sv_find_by_delim with index = 2_1. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected2_1), ARG_SV(testFile2_1));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_by_delim with index = 2_1.");
    }

    StringView testFile2_2 = sv_find_by_delim(testPath2, ':', 2);
    StringView expected2_2 = cstr_sv("two");
    if(!sv_same(testFile2_2, expected2_2))
    {
        PD_FAIL("sv_find_by_delim with index = 2_2. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected2_2), ARG_SV(testFile2_2));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_by_delim with index = 2_2.");
    }

    uint32_t result = sv_count_by_delim(testPath, ';');
    if(result != 5)
    {
        PD_FAIL("sv_count_by_delim in testPath. expected: 5\ngot: %u\n", result);
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_count_by_delim unit test with testPath.");
    }

    result = sv_count_by_delim(testPath2, ':');
    if(result != 3)
    {
        PD_FAIL("sv_count_by_delim in testPath2. expected: 3\ngot: %u\n", result);
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_count_by_delim unit test with testPath2.");
    }

    StringView premade[6] =
    {
        cstr_sv("test1"),
        cstr_sv("2"),
        cstr_sv("third"),
        cstr_sv("fourth"),
        cstr_sv("five"),
        cstr_sv("sixth_last!;")
    };

    StringView separate_sv = cstr_sv(":test1:2::third:fourth:five:::sixth_last!;");
    uint32_t   itemcount   = sv_count_by_delim(separate_sv, ':');
    StringView separated_list[itemcount];
    sv_separate_by_delim(separate_sv, separated_list, ':');

    if(itemcount != 6)
    {
        PD_FAIL("sv_count_by_delim in separate_sv. expected: 6\ngot: %u\n", result);
        ++num_failed;
    }

    uint8_t sep_i = 0;
    for(; sep_i < itemcount; ++sep_i)
    {
        if(!sv_same(premade[sep_i], separated_list[sep_i]))
        {
            PD_FAIL("sv_count_by_delim in separate_sv. expected: "PRI_SV"\ngot: "
                    PRI_SV"\n", ARG_SV(premade[sep_i]), ARG_SV(separated_list[sep_i]));
            ++num_failed;
            break;
        }
    }

    if(sep_i == 6)
    {
        PD_SUCCESS("passed sv_separate_by_delim with separate_sv.");
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
        PD_FAIL("sv_is_lesser with \"..\" vs \".\". expected: 0\ngot: 1\n");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_is_lesser with \"..\" vs \".\".");
    }

    StringView a1 = cstr_sv("123");
    StringView b1 = cstr_sv("ab");
    if(sv_is_lesser(b1, a1))
    {
        PD_FAIL("sv_is_lesser with \"123\" vs \"abc\". expected: 0\ngot: 1\n");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_is_lesser with \"123\" vs \"abc\".");
    }

    result = sv_count_by_delim(testPath_sorted, ':');
    if(result != 4)
    {
        PD_FAIL("sv_count_by_delim with testPath_sorted. "
                "expected: 4\ngot: %u\n", result);
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_count_by_delim with testPath_sorted.");
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

    StringView one_dot = cstr_sv(".");
    StringView two_dot = cstr_sv("..");
    StringView one_test;
    StringView two_test;
    one_test = sv_find_by_delim(testPath_real, ';', 0);
    two_test = sv_find_by_delim(testPath_real, ';', 1);

    if(!sv_same(one_test, one_dot))
    {
        PD_FAIL("sv_sort_by_delim with real path and the '..' identifier."
                "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(one_dot), ARG_SV(one_test));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_sort_by_delim with real path and the '.' identifier.");
    }

    if(!sv_same(two_test, two_dot))
    {
        PD_FAIL("sv_sort_by_delim with real path and the '..' identifier."
                "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(two_dot), ARG_SV(two_test));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_sort_by_delim with real path and the '..' identifier.");
    }

    StringView expected_sort0 = cstr_sv("123");
    StringView testFile_sort0 = sv_find_by_delim(testPath_sorted, ':', 0);
    if(!sv_same(testFile_sort0, expected_sort0))
    {
        PD_FAIL("sv_find_by_delim with index = 0. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected_sort0), ARG_SV(testFile_sort0));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_by_delim with index = 0.");
    }

    StringView expected_sort1 = cstr_sv("aaa");
    StringView testFile_sort1 = sv_find_by_delim(testPath_sorted, ':', 1);
    if(!sv_same(testFile_sort1, expected_sort1))
    {
        PD_FAIL("sv_find_by_delim with index = 1. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected_sort1), ARG_SV(testFile_sort1));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_by_delim with index = 1.");
    }

    StringView expected_sort2 = cstr_sv("abc");
    StringView testFile_sort2 = sv_find_by_delim(testPath_sorted, ':', 2);
    if(!sv_same(testFile_sort2, expected_sort2))
    {
        PD_FAIL("sv_find_by_delim with index = 2. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected_sort2), ARG_SV(testFile_sort2));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_by_delim with index = 2.");
    }

    StringView expected_sort3 = cstr_sv("abd");
    StringView testFile_sort3 = sv_find_by_delim(testPath_sorted, ':', 3);
    if(!sv_same(testFile_sort2, expected_sort2))
    {
        PD_FAIL("sv_find_by_delim with index = 3. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected_sort3), ARG_SV(testFile_sort3));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed sv_find_by_delim with index = 3.");
    }

    float testValsFloat[10] =
    {
        0.1f, 0.22f, 0.333f, 0.4444f, 0.5f,
        69.120f, 111001.12f, 199.99999f, 2.13f, 3.149175f
    };

    float *arr_float = 0;
    for(uint32_t i = 0; i < 10; ++i)
    {
        pdArrPush(arr_float, testValsFloat[i]);
    }

    uint8_t failed = 0;
    for(uint32_t i = 0; i < pdArrSize(arr_float); ++i)
    {
        if(testValsFloat[i] != arr_float[i])
        {
            PD_FAIL("pdArrPush with type uint32_t. expected: %f\ngot: %f\n",
                    testValsFloat[i], arr_float[i]);
            ++num_failed;
            failed = 1;
        }
    }
    if(!failed)
    {
        PD_SUCCESS("passed pdPushArr with float.");
    }
    pdArrFree(arr_float);

    uint32_t testVals32[10] =
    {
        65535, 10597, 19, 192856, 19285,
        198, 19285615, 918, 19285, 1509
    };

    pdArr(uint32_t) arr_uint = 0;
    pdArrReserve(arr_uint, 5);
    for(uint32_t i = 0; i < 10; ++i)
    {
        pdArrPush(arr_uint, testVals32[i]);
    }

    failed = 0;
    for(uint32_t i = 0; i < pdArrSize(arr_uint); ++i)
    {
        if(testVals32[i] != arr_uint[i])
        {
            PD_FAIL("pdArrPush with type uint32_t. expected: %u\ngot: %u\n",
                    testVals32[i], arr_uint[i]);
            ++num_failed;
            failed = 1;
        }
    }
    if(!failed)
    {
        PD_SUCCESS("passed pdPushArr with uint32_t.");
    }
    pdArrFree(arr_uint);

    if(num_failed)
    {
        PD_ERROR("failed %i unit tests.", num_failed);
    }

    char buf[4096 * 12];
    pdListFiles(cstr_sv("."), buf);

    return num_failed;
}
