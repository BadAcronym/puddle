#include "pd_path.h"
#include "pd_dyn_arr.h"
#include "pd_string_view.h"
#include "pd_print_macros.h"

int main
(
    void
){
    StringView test        = pdCstrSV("Hello, World!");
    StringView substr      = pdSVSubstr(&test, 7, test.size);
    StringView substr_test = pdCstrSV("World!");
    StringView sub_test_0  = pdCstrSV("W");
    StringView sub_test_1  = pdSVSubstr(&test, 7, 7);
    StringView test_left   = pdCstrSV("lo, World!");
    StringView test_right  = pdCstrSV("lo, Worl");
    StringView test_both   = pdCstrSV(" W");
    StringView conv_src    = pdCstrSV("The quick lilac fox jumps over the dog.");
    char conv_test[conv_src.size + 1];
    pdSVCstr(conv_src, conv_test);
    StringView conv_test2  = pdCstrSV(conv_test);
    StringView concat_1    = pdCstrSV("Test ");
    StringView concat_2    = pdCstrSV("concat !");
    StringView concat_r    = pdCstrSV("Test concat !");

    int num_failed = 0;

    if(!pdSVSame(conv_src, conv_test2))
    {
        PD_FAIL("pdSVCstr. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                 ARG_SV(conv_src), ARG_SV(conv_test2));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVCstr.");
    }

    if(!pdSVSame(substr, substr_test))
    {
        PD_FAIL("pdSVSubstr. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(substr_test), ARG_SV(substr));
        PD_DEBUG("%zu vs %zu", substr_test.size, substr.size);
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVSubstr.");
    }

    if(!pdSVSame(sub_test_0, sub_test_1))
    {
        PD_FAIL("pdSVSubstr on a character. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(sub_test_0), ARG_SV(sub_test_1));
        PD_DEBUG("%zu vs %zu", substr_test.size, substr.size);
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVSubstr on a character.");
    }

    pdSVTrim(&test, 3, SV_LEFT);
    if(!pdSVSame(test, test_left))
    {
        PD_FAIL("pdSVTrim with SV_LEFT. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(test_left), ARG_SV(test));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVTrim with SV_LEFT.");
    }

    pdSVTrim(&test, 2, SV_RIGHT);
    if(!pdSVSame(test, test_right))
    {
        PD_FAIL("pdSVTrim with SV_RIGHT. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(test_right), ARG_SV(test));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVTrim with SV_RIGHT.");
    }

    pdSVTrim(&test, 3, SV_BOTH);
    if(!pdSVSame(test, test_both))
    {
        PD_FAIL("pdSVTrim with SV_BOTH. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(test_both), ARG_SV(test));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVTrim with SV_BOTH.");
    }

    char concatenated[concat_1.size + concat_2.size + 1];
    pdSVConcat(concat_1, concat_2, concatenated);
    StringView test_concat = pdCstrSV(concatenated);
    if(!pdSVSame(concat_r, test_concat))
    {
        PD_FAIL("pdSVConcat. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(concat_r), ARG_SV(test_concat));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVConcat unit test.");
    }

    StringView bigStr = pdCstrSV("The quick brown fox jumps over the lazy dog.");
    StringView small0 = pdCstrSV("The ");
    StringView small1 = pdCstrSV(" The");
    StringView small2 = pdCstrSV(" jumps ");
    StringView small3 = pdCstrSV("dog.");
    StringView small4 = pdCstrSV(".");

    if(pdSVIsSubstr(small1, bigStr))
    {
        PD_FAIL("pdSVIsSubstr with SV_DIFFERENT.");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVIsSubstr with SV_DIFFERENT.");
    }

    if(!pdSVIsSubstr(small0, bigStr))
    {
        PD_FAIL("pdSVIsSubstr with SV_IS_SUBSTR, 1.");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVIsSubstr with SV_IS_SUBSTR, 1.");
    }

    if(!pdSVIsSubstr(small2, bigStr))
    {
        PD_FAIL("pdSVIsSubstr with SV_IS_SUBSTR, 2.");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVIsSubstr with SV_IS_SUBSTR, 2.");
    }

    if(!pdSVIsSubstr(small3, bigStr))
    {
        PD_FAIL("pdSVIsSubstr with SV_IS_SUBSTR, 3.");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVIsSubstr with SV_IS_SUBSTR, 3.");
    }

    if(pdSVFind(small1, bigStr))
    {
        PD_FAIL("pdSVFind with non-findable pattern.");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFind with non-findable pattern.");
    }

    const char *found = pdSVFind(small0, bigStr);
    if(!found)
    {
        PD_FAIL("pdSVFind with findable pattern, 1.");
        PD_ERROR("failed to find pattern entirely.");
        ++num_failed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small0.size)
    {
        PD_FAIL("pdSVFind with findable pattern, 1.");
        PD_ERROR("returned an invalid pointer. StringView sv valid range: %p - %p.\n"
                 "Returned pointer was: %p.\nThat's %li away from the start and "
                 "%li away from the end.\n",
                 bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                 bigStr.data + bigStr.size - found);
        ++num_failed;
    }
    else if(found != bigStr.data)
    {
        PD_FAIL("pdSVFind with findable pattern, 1.");
        PD_ERROR("returned the wrong offset. Expected: 0, got: %u\033[0m\n",
                (uint32_t)(found - bigStr.data));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFind with findable pattern, 1.");
    }

    found = pdSVFind(small2, bigStr);
    if(!found)
    {
        PD_FAIL("pdSVFind with findable pattern, 2.");
        PD_ERROR("failed to find pattern entirely.");
        ++num_failed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small2.size)
    {
        PD_FAIL("pdSVFind with findable pattern, 2.");
        PD_ERROR("returned an invalid pointer. StringView sv valid range: %p - %p.\n"
                 "Returned pointer was: %p.\nThat's %li away from the start and "
                 "%li away from the end.\n",
                 bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                 bigStr.data + bigStr.size - found);
        ++num_failed;
    }
    else if(found != bigStr.data + 19)
    {
        PD_FAIL("pdSVFind with findable pattern, 2.");
        PD_ERROR("returned the wrong offset. Expected: 19, got: %u\033[0m\n",
                (uint32_t)(found - bigStr.data));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFind with findable pattern, 2.");
    }

    found = pdSVFind(small3, bigStr);
    if(!found)
    {
        PD_FAIL("pdSVFind with findable pattern, 3.");
        PD_ERROR("failed to find pattern entirely.");
        ++num_failed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small3.size)
    {
        PD_FAIL("pdSVFind with findable pattern, 3.");
        PD_ERROR("returned an invalid pointer. StringView sv valid range: %p - %p.\n"
                 "Returned pointer was: %p.\nThat's %li away from the start and "
                 "%li away from the end.\n",
                 bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                 bigStr.data + bigStr.size - found);
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFind with findable pattern, 3.");
    }

    found = pdSVFind(small4, bigStr);
    if(!found)
    {
        PD_FAIL("pdSVFind with findable pattern, 4.");
        PD_ERROR("failed to find pattern entirely.");
        ++num_failed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small4.size)
    {
        PD_FAIL("pdSVFind with findable pattern, 4.");
        PD_ERROR("returned an invalid pointer. StringView sv valid range: %p - %p.\n"
                 "Returned pointer was: %p.\nThat's %li away from the start and "
                 "%li away from the end.\n",
                 bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                 bigStr.data + bigStr.size - found);
        ++num_failed;
    }
    else if(found != bigStr.data + 43)
    {
        PD_FAIL("pdSVFind with findable pattern, 4.");
        PD_ERROR("returned the wrong offset. Expected: 43, got: %u\033[0m\n",
                (uint32_t)(found - bigStr.data));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFind with findable pattern, 4.");
    }

    found = pdSVFindLast(small4, bigStr);
    if(found != bigStr.data + 43)
    {
        PD_FAIL("pdSVFindLast with findable pattern, 1.");
        PD_ERROR("returned the wrong offset. Expected: 43, got: %u\033[0m\n",
                (uint32_t)(found - bigStr.data));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindLast with findable pattern, 1.");
    }

    StringView duplStr = pdCstrSV("The fox jumps jumps jumps jumps");
    found = pdSVFindLast(small2, duplStr);
    if(found != duplStr.data + 19)
    {
        PD_FAIL("pdSVFindLast with findable pattern, 2.");
        PD_ERROR("returned the wrong offset. Expected: 19, got: %u\033[0m\n",
                (uint32_t)(found - duplStr.data));
    }
    else
    {
        PD_SUCCESS("passed pdSVFindLast with findable pattern, 2.");
    }

    StringView testPath = pdCstrSV(";zeroeth;;first;second;third;;;;;;;;fourth;");

    StringView testFile0 = pdSVFindByDelim(testPath, ';', 0);
    StringView expected0 = pdCstrSV("zeroeth");
    if(!pdSVSame(testFile0, expected0))
    {
        PD_FAIL("pdSVFindByDelim with index = 0. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected0), ARG_SV(testFile0));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 0.");
    }

    StringView testFile1 = pdSVFindByDelim(testPath, ';', 1);
    StringView expected1 = pdCstrSV("first");
    if(!pdSVSame(testFile1, expected1))
    {
        PD_FAIL("pdSVFindByDelim with index = 1. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected1), ARG_SV(testFile1));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 1.");
    }

    StringView testFile2 = pdSVFindByDelim(testPath, ';', 2);
    StringView expected2 = pdCstrSV("second");
    if(!pdSVSame(testFile2, expected2))
    {
        PD_FAIL("pdSVFindByDelim with index = 2. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected2), ARG_SV(testFile2));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 2.");
    }

    StringView testFile3 = pdSVFindByDelim(testPath, ';', 3);
    StringView expected3 = pdCstrSV("third");
    if(!pdSVSame(testFile3, expected3))
    {
        PD_FAIL("pdSVFindByDelim with index = 3. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected3), ARG_SV(testFile3));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 3.");
    }

    StringView testFile4 = pdSVFindByDelim(testPath, ';', 4);
    StringView expected4 = pdCstrSV("fourth");
    if(!pdSVSame(testFile4, expected4))
    {
        PD_FAIL("pdSVFindByDelim with index = 4. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected4), ARG_SV(testFile4));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 4.");
    }

    StringView testPath2 = pdCstrSV("zero:one:two::::");

    StringView testFile2_0 = pdSVFindByDelim(testPath2, ':', 0);
    StringView expected2_0 = pdCstrSV("zero");
    if(!pdSVSame(testFile2_0, expected2_0))
    {
        PD_FAIL("pdSVFindByDelim with index = 2_0. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected2_0), ARG_SV(testFile2_0));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 2_0.");
    }

    StringView testFile2_1 = pdSVFindByDelim(testPath2, ':', 1);
    StringView expected2_1 = pdCstrSV("one");
    if(!pdSVSame(testFile2_1, expected2_1))
    {
        PD_FAIL("pdSVFindByDelim with index = 2_1. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected2_1), ARG_SV(testFile2_1));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 2_1.");
    }

    StringView testFile2_2 = pdSVFindByDelim(testPath2, ':', 2);
    StringView expected2_2 = pdCstrSV("two");
    if(!pdSVSame(testFile2_2, expected2_2))
    {
        PD_FAIL("pdSVFindByDelim with index = 2_2. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected2_2), ARG_SV(testFile2_2));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 2_2.");
    }

    uint32_t result = pdSVCountByDelim(testPath, ';');
    if(result != 5)
    {
        PD_FAIL("pdSVCountByDelim in testPath. expected: 5\ngot: %u\n", result);
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVCountByDelim unit test with testPath.");
    }

    result = pdSVCountByDelim(testPath2, ':');
    if(result != 3)
    {
        PD_FAIL("pdSVCountByDelim in testPath2. expected: 3\ngot: %u\n", result);
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVCountByDelim unit test with testPath2.");
    }

    StringView premade[6] =
    {
        pdCstrSV("test1"),
        pdCstrSV("2"),
        pdCstrSV("third"),
        pdCstrSV("fourth"),
        pdCstrSV("five"),
        pdCstrSV("sixth_last!;")
    };

    StringView separate_sv = pdCstrSV(":test1:2::third:fourth:five:::sixth_last!;");
    uint32_t   itemcount   = pdSVCountByDelim(separate_sv, ':');
    StringView separated_list[itemcount];
    pdSVSeparateByDelim(separate_sv, separated_list, ':', itemcount);

    if(itemcount != 6)
    {
        PD_FAIL("pdSVCountByDelim in separate_sv. expected: 6\ngot: %u\n", result);
        ++num_failed;
    }

    uint8_t sep_i = 0;
    for(; sep_i < itemcount; ++sep_i)
    {
        if(!pdSVSame(premade[sep_i], separated_list[sep_i]))
        {
            PD_FAIL("pdSVCountByDelim in separate_sv. expected: "PRI_SV"\ngot: "
                    PRI_SV"\n", ARG_SV(premade[sep_i]), ARG_SV(separated_list[sep_i]));
            ++num_failed;
            break;
        }
    }

    if(sep_i == 6)
    {
        PD_SUCCESS("passed pdSVSeparateByDelim with separate_sv.");
    }

    StringView testPath_tosort = pdCstrSV("abc:abd:123:aaa");
    char sorted[testPath_tosort.size + 1];
    pdSVSortByDelim(testPath_tosort, ':', sorted);

    StringView testPath_sorted =
    {
        .data = sorted,
        .size = testPath_tosort.size
    };

    StringView a = pdCstrSV("..");
    StringView b = pdCstrSV(".");
    if(pdSVIsLesser(a, b))
    {
        PD_FAIL("pdSVIsLesser with \"..\" vs \".\". expected: 0\ngot: 1\n");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVIsLesser with \"..\" vs \".\".");
    }

    StringView a1 = pdCstrSV("123");
    StringView b1 = pdCstrSV("ab");
    if(pdSVIsLesser(b1, a1))
    {
        PD_FAIL("pdSVIsLesser with \"123\" vs \"abc\". expected: 0\ngot: 1\n");
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVIsLesser with \"123\" vs \"abc\".");
    }

    result = pdSVCountByDelim(testPath_sorted, ':');
    if(result != 4)
    {
        PD_FAIL("pdSVCountByDelim with testPath_sorted. "
                "expected: 4\ngot: %u\n", result);
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVCountByDelim with testPath_sorted.");
    }

    StringView testPath_real = pdCstrSV("32 32 textures 2.qoi;"
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
    pdSVSortByDelim(testPath_real, ';', sorted_real);
    testPath_real.data = sorted_real;

    StringView one_dot = pdCstrSV(".");
    StringView two_dot = pdCstrSV("..");
    StringView one_test;
    StringView two_test;
    one_test = pdSVFindByDelim(testPath_real, ';', 0);
    two_test = pdSVFindByDelim(testPath_real, ';', 1);

    if(!pdSVSame(one_test, one_dot))
    {
        PD_FAIL("pdSVSortByDelim with real path and the '..' identifier."
                "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(one_dot), ARG_SV(one_test));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVSortByDelim with real path and the '.' identifier.");
    }

    if(!pdSVSame(two_test, two_dot))
    {
        PD_FAIL("pdSVSortByDelim with real path and the '..' identifier."
                "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(two_dot), ARG_SV(two_test));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVSortByDelim with real path and the '..' identifier.");
    }

    StringView expected_sort0 = pdCstrSV("123");
    StringView testFile_sort0 = pdSVFindByDelim(testPath_sorted, ':', 0);
    if(!pdSVSame(testFile_sort0, expected_sort0))
    {
        PD_FAIL("pdSVFindByDelim with index = 0. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected_sort0), ARG_SV(testFile_sort0));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 0.");
    }

    StringView expected_sort1 = pdCstrSV("aaa");
    StringView testFile_sort1 = pdSVFindByDelim(testPath_sorted, ':', 1);
    if(!pdSVSame(testFile_sort1, expected_sort1))
    {
        PD_FAIL("pdSVFindByDelim with index = 1. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected_sort1), ARG_SV(testFile_sort1));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 1.");
    }

    StringView expected_sort2 = pdCstrSV("abc");
    StringView testFile_sort2 = pdSVFindByDelim(testPath_sorted, ':', 2);
    if(!pdSVSame(testFile_sort2, expected_sort2))
    {
        PD_FAIL("pdSVFindByDelim with index = 2. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected_sort2), ARG_SV(testFile_sort2));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 2.");
    }

    StringView expected_sort3 = pdCstrSV("abd");
    StringView testFile_sort3 = pdSVFindByDelim(testPath_sorted, ':', 3);
    if(!pdSVSame(testFile_sort2, expected_sort2))
    {
        PD_FAIL("pdSVFindByDelim with index = 3. expected: \""PRI_SV"\"\ngot: \""
                PRI_SV"\"\n", ARG_SV(expected_sort3), ARG_SV(testFile_sort3));
        ++num_failed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 3.");
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
    pdListFiles(pdCstrSV("."), buf);

    return num_failed;
}
