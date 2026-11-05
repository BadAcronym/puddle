#include "pd_path.h"
#include "pd_dyn_arr.h"
#include "pd_int_types.h"
#include "pd_string_view.h"
#include "pd_print_macros.h"

int main
(
    void
){
    StringView test       = pdCstrSV("Hello, World!");
    StringView substr     = pdSVSubstr(&test, 7, test.size);
    StringView substrTest = pdCstrSV("World!");
    StringView subest0    = pdCstrSV("W");
    StringView subest1    = pdSVSubstr(&test, 7, 7);
    StringView testLeft   = pdCstrSV("lo, World!");
    StringView testRight  = pdCstrSV("lo, Worl");
    StringView testBoth   = pdCstrSV(" W");
    StringView convSrc    = pdCstrSV("The quick lilac fox jumps over the dog.");
    char convTest[convSrc.size + 1];
    pdSVCstr(convSrc, convTest);
    StringView convTest2  = pdCstrSV(convTest);
    StringView concat1    = pdCstrSV("Test ");
    StringView concat2    = pdCstrSV("concat !");
    StringView concatR    = pdCstrSV("Test concat !");

    int numFailed = 0;

    if(!pdSVSame(convSrc, convTest2))
    {
        PD_FAIL("pdSVCstr. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                 ARG_SV(convSrc), ARG_SV(convTest2));
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVCstr.");
    }

    if(!pdSVSame(substr, substrTest))
    {
        PD_FAIL("pdSVSubstr. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(substrTest), ARG_SV(substr));
        PD_DEBUG("%"PRIu64" vs %"PRIu64"", substrest.size, substr.size);
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVSubstr.");
    }

    if(!pdSVSame(subest0, subest1))
    {
        PD_FAIL("pdSVSubstr on a character. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(subest0), ARG_SV(subest1));
        PD_DEBUG("%"PRIu64" vs %"PRIu64"", substrest.size, substr.size);
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVSubstr on a character.");
    }

    pdSVTrim(&test, 3, SV_LEFT);
    if(!pdSVSame(test, testLeft))
    {
        PD_FAIL("pdSVTrim with SV_LEFT. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(testLeft), ARG_SV(test));
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVTrim with SV_LEFT.");
    }

    pdSVTrim(&test, 2, SV_RIGHT);
    if(!pdSVSame(test, testRight))
    {
        PD_FAIL("pdSVTrim with SV_RIGHT. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(testRight), ARG_SV(test));
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVTrim with SV_RIGHT.");
    }

    pdSVTrim(&test, 3, SV_BOTH);
    if(!pdSVSame(test, testBoth))
    {
        PD_FAIL("pdSVTrim with SV_BOTH. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(testBoth), ARG_SV(test));
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVTrim with SV_BOTH.");
    }

    char concatenated[concat1.size + concat2.size + 1];
    pdSVConcat(concat1, concat2, concatenated);
    StringView test_concat = pdCstrSV(concatenated);
    if(!pdSVSame(concatR, test_concat))
    {
        PD_FAIL("pdSVConcat. expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"",
                ARG_SV(concatR), ARG_SV(test_concat));
        ++numFailed;
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
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVIsSubstr with SV_DIFFERENT.");
    }

    if(!pdSVIsSubstr(small0, bigStr))
    {
        PD_FAIL("pdSVIsSubstr with SV_IS_SUBSTR, 1.");
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVIsSubstr with SV_IS_SUBSTR, 1.");
    }

    if(!pdSVIsSubstr(small2, bigStr))
    {
        PD_FAIL("pdSVIsSubstr with SV_IS_SUBSTR, 2.");
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVIsSubstr with SV_IS_SUBSTR, 2.");
    }

    if(!pdSVIsSubstr(small3, bigStr))
    {
        PD_FAIL("pdSVIsSubstr with SV_IS_SUBSTR, 3.");
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVIsSubstr with SV_IS_SUBSTR, 3.");
    }

    if(pdSVFind(small1, bigStr))
    {
        PD_FAIL("pdSVFind with non-findable pattern.");
        ++numFailed;
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
        ++numFailed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small0.size)
    {
        PD_FAIL("pdSVFind with findable pattern, 1.");
        PD_ERROR("returned an invalid pointer. StringView sv valid range: %p - %p.\n"
                 "Returned pointer was: %p.\nThat's %"PRIi64" away from the start and "
                 "%"PRIi64" away from the end.\n",
                 bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                 bigStr.data + bigStr.size - found);
        ++numFailed;
    }
    else if(found != bigStr.data)
    {
        PD_FAIL("pdSVFind with findable pattern, 1.");
        PD_ERROR("returned the wrong offset. Expected: 0, got: %"PRIu32"\033[0m\n",
                (u32)(found - bigStr.data));
        ++numFailed;
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
        ++numFailed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small2.size)
    {
        PD_FAIL("pdSVFind with findable pattern, 2.");
        PD_ERROR("returned an invalid pointer. StringView sv valid range: %p - %p.\n"
                 "Returned pointer was: %p.\nThat's %"PRIi64" away from the start and "
                 "%"PRIi64" away from the end.\n",
                 bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                 bigStr.data + bigStr.size - found);
        ++numFailed;
    }
    else if(found != bigStr.data + 19)
    {
        PD_FAIL("pdSVFind with findable pattern, 2.");
        PD_ERROR("returned the wrong offset. Expected: 19, got: %"PRIu32"\033[0m\n",
                (u32)(found - bigStr.data));
        ++numFailed;
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
        ++numFailed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small3.size)
    {
        PD_FAIL("pdSVFind with findable pattern, 3.");
        PD_ERROR("returned an invalid pointer. StringView sv valid range: %p - %p.\n"
                 "Returned pointer was: %p.\nThat's %"PRIi64" away from the start and "
                 "%"PRIi64" away from the end.\n",
                 bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                 bigStr.data + bigStr.size - found);
        ++numFailed;
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
        ++numFailed;
    }
    if(found < bigStr.data || found > bigStr.data + bigStr.size - small4.size)
    {
        PD_FAIL("pdSVFind with findable pattern, 4.");
        PD_ERROR("returned an invalid pointer. StringView sv valid range: %p - %p.\n"
                 "Returned pointer was: %p.\nThat's %"PRIi64" away from the start and "
                 "%"PRIi64" away from the end.\n",
                 bigStr.data, bigStr.data + bigStr.size, found, bigStr.data - found,
                 bigStr.data + bigStr.size - found);
        ++numFailed;
    }
    else if(found != bigStr.data + 43)
    {
        PD_FAIL("pdSVFind with findable pattern, 4.");
        PD_ERROR("returned the wrong offset. Expected: 43, got: %"PRIu32"\033[0m\n",
                (u32)(found - bigStr.data));
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFind with findable pattern, 4.");
    }

    found = pdSVFindLast(small4, bigStr);
    if(found != bigStr.data + 43)
    {
        PD_FAIL("pdSVFindLast with findable pattern, 1.");
        PD_ERROR("returned the wrong offset. Expected: 43, got: %"PRIu32"\033[0m\n",
                (u32)(found - bigStr.data));
        ++numFailed;
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
        PD_ERROR("returned the wrong offset. Expected: 19, got: %"PRIu32"\033[0m\n",
                (u32)(found - duplStr.data));
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
        ++numFailed;
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
        ++numFailed;
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
        ++numFailed;
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
        ++numFailed;
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
        ++numFailed;
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
        ++numFailed;
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
        ++numFailed;
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
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 2_2.");
    }

    u32 result = pdSVCountByDelim(testPath, ';');
    if(result != 5)
    {
        PD_FAIL("pdSVCountByDelim in testPath. expected: 5\ngot: %"PRIu32"\n", result);
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVCountByDelim unit test with testPath.");
    }

    result = pdSVCountByDelim(testPath2, ':');
    if(result != 3)
    {
        PD_FAIL("pdSVCountByDelim in testPath2. expected: 3\ngot: %"PRIu32"\n", result);
        ++numFailed;
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
    u32   itemcount   = pdSVCountByDelim(separate_sv, ':');
    StringView separated_list[itemcount];
    pdSVSeparateByDelim(separate_sv, separated_list, ':', itemcount);

    if(itemcount != 6)
    {
        PD_FAIL("pdSVCountByDelim in separate_sv. expected: 6\ngot: %"PRIu32"\n",
                result);
        ++numFailed;
    }

    u08 sep_i = 0;
    for(; sep_i < itemcount; ++sep_i)
    {
        if(!pdSVSame(premade[sep_i], separated_list[sep_i]))
        {
            PD_FAIL("pdSVCountByDelim in separate_sv. expected: "PRI_SV"\ngot: "
                    PRI_SV"\n", ARG_SV(premade[sep_i]), ARG_SV(separated_list[sep_i]));
            ++numFailed;
            break;
        }
    }

    if(sep_i == 6)
    {
        PD_SUCCESS("passed pdSVSeparateByDelim with separate_sv.");
    }

    StringView testPathosort = pdCstrSV("abc:abd:123:aaa");
    char sorted[testPathosort.size + 1];
    pdSVSortByDelim(testPathosort, ':', sorted);

    StringView testPath_sorted =
    {
        .data = sorted,
        .size = testPathosort.size
    };

    StringView a = pdCstrSV("..");
    StringView b = pdCstrSV(".");
    if(pdSVIsLesser(a, b))
    {
        PD_FAIL("pdSVIsLesser with \"..\" vs \".\". expected: 0\ngot: 1\n");
        ++numFailed;
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
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVIsLesser with \"123\" vs \"abc\".");
    }

    result = pdSVCountByDelim(testPath_sorted, ':');
    if(result != 4)
    {
        PD_FAIL("pdSVCountByDelim with testPath_sorted. expected: 4\ngot: %"
                PRIu32"\n", result);
        ++numFailed;
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
    StringView oneest;
    StringView twoest;
    oneest = pdSVFindByDelim(testPath_real, ';', 0);
    twoest = pdSVFindByDelim(testPath_real, ';', 1);

    if(!pdSVSame(oneest, one_dot))
    {
        PD_FAIL("pdSVSortByDelim with real path and the '..' identifier."
                "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(one_dot), ARG_SV(oneest));
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVSortByDelim with real path and the '.' identifier.");
    }

    if(!pdSVSame(twoest, two_dot))
    {
        PD_FAIL("pdSVSortByDelim with real path and the '..' identifier."
                "expected: \""PRI_SV"\"\ngot: \""PRI_SV"\"\n",
                ARG_SV(two_dot), ARG_SV(twoest));
        ++numFailed;
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
        ++numFailed;
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
        ++numFailed;
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
        ++numFailed;
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
        ++numFailed;
    }
    else
    {
        PD_SUCCESS("passed pdSVFindByDelim with index = 3.");
    }

    f32 testValsF32[10] =
    {
        0.1f, 0.22f, 0.333f, 0.4444f, 0.5f,
        69.120f, 111001.12f, 199.99999f, 2.13f, 3.149175f
    };

    f32 *arrF32 = 0;
    for(u32 i = 0; i < 10; ++i)
    {
        pdArrPush(arrF32, testValsF32[i]);
    }

    u08 failed = 0;
    for(u32 i = 0; i < pdArrSize(arrF32); ++i)
    {
        if(testValsF32[i] != arrF32[i])
        {
            PD_FAIL("pdArrPush with type u32. expected: %f\ngot: %f\n",
                    testValsF32[i], arrF32[i]);
            ++numFailed;
            failed = 1;
        }
    }
    if(!failed)
    {
        PD_SUCCESS("passed pdPushArr with f32.");
    }
    pdArrFree(arrF32);

    u32 testValsU32[10] =
    {
        65535, 10597, 19, 192856, 19285,
        198, 19285615, 918, 19285, 1509
    };

    pdArr(u32) arrU32 = 0;
    pdArrReserve(arrU32, 5);
    for(u32 i = 0; i < 10; ++i)
    {
        pdArrPush(arrU32, testValsU32[i]);
    }

    failed = 0;
    for(u32 i = 0; i < pdArrSize(arrU32); ++i)
    {
        if(testValsU32[i] != arrU32[i])
        {
            PD_FAIL("pdArrPush with type u32. expected: %"PRIu32"\ngot: %"PRIu32
                    "\n", testValsU32[i], arrU32[i]);
            ++numFailed;
            failed = 1;
        }
    }
    if(!failed)
    {
        PD_SUCCESS("passed pdPushArr with u32.");
    }
    pdArrFree(arrU32);

    if(numFailed)
    {
        PD_ERROR("failed %i unit tests.", numFailed);
    }

    char buf[4096 * 12];
    pdListFiles(pdCstrSV("."), buf);

    return numFailed;
}
