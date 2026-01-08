#ifndef TEST_LB_STRING_H
#define TEST_LB_STRING_H

#include "munit.h"
#include "lb_sysi/lb_string.h"

static MunitResult test_string_count_vis (const MunitParameter params[],
                                          void * data) {
        int result;
        munit_assert_int(string_count_vis(&result, "Hello"), ==, 0);
        munit_assert_int(result, ==, 5);
        munit_assert_int(string_count_vis(&result, ""), ==, 0);
        munit_assert_int(result, ==, 0);
        char * tstr = LBF_RESET "TEST!";
        munit_assert_int(string_count_vis(&result, tstr), ==, 0);
        munit_assert_int(result, ==, 5);
        return MUNIT_OK;
};

static MunitResult test_string_count_size (const MunitParameter params[],
                                           void * data) {
        int result;
        munit_assert_int(string_count_size(&result, "Hello"), ==, 0);
        munit_assert_int(result, ==, 6);
        munit_assert_int(string_count_size(&result, ""), ==, 0);
        munit_assert_int(result, ==, 1);
        char * tstr = LBF_RESET "TEST!";
        munit_assert_int(string_count_size(&result, tstr), ==, 0);
        munit_assert_int(result, ==, 16);
        return MUNIT_OK;
};

static MunitResult test_string_is_equal (const MunitParameter params[],
                                         void * data) {
        char * str1 = "Hello!";
        char * str2 = "Goodbye.";
        char * str3 = "Hello!";
        munit_assert_false(string_is_equal(str1, str2));
        munit_assert_true(string_is_equal(str1, str3));
        munit_assert_true(string_is_equal(str1, str1));
        munit_assert_true(string_is_equal(str1, "Hello!"));
        munit_assert_false(string_is_equal(str1, "Goodbye."));
        munit_assert_true(string_is_equal("Test.", "Test."));
        munit_assert_false(string_is_equal("Hello.", "Goodbye."));
        munit_assert_true(string_is_equal("Test.\0I'm extra!", "Test."));
        return MUNIT_OK;
};

static MunitResult test_string_del (const MunitParameter params[],
                                    void * data) {
        char * str = malloc(32 * sizeof(char));
        munit_assert_int(string_cat(str, "Hello, lousy world!"), ==, 0);
        int index, count;
        index = 0;
        count = 0;
        munit_assert_int(string_del(str, &index, &count), ==, 0);
        munit_assert_string_equal(str, str);
        index = 7;
        count = 6;
        munit_assert_int(string_del(str, &index, &count), ==, 0);
        munit_assert_string_equal(str, "Hello, world!");
        free(str);
        return MUNIT_OK;
};

static MunitResult test_string_ins_char (const MunitParameter params[],
                                         void * data) {
        char * str = malloc(32 * sizeof(char));
        str[0] = LBF_NULL;
        int i = 0;
        char c = 'X';
        munit_assert_int(string_ins_char(str, &i, &c), ==, 0);
        munit_assert_string_equal(str, "X");
        i++;
        c = 'Y';
        munit_assert_int(string_ins_char(str, &i, &c), ==, 0);
        munit_assert_string_equal(str, "XY");
        c = 'Z';
        munit_assert_int(string_ins_char(str, &i, &c), ==, 0);
        munit_assert_string_equal(str, "XZY");
        free(str);
        return MUNIT_OK;
};

static MunitResult test_string_ins_string (const MunitParameter params[],
                                           void * data) {
        char * str1 = calloc(32, sizeof(char));
        char * str2 = calloc(16, sizeof(char));
        int di = 0;
        int si = 0;
        int sc = 5;
        munit_assert_int(string_cat(str2, "Hello"), ==, 0);
        munit_assert_string_equal(str1, "");
        munit_assert_int(string_ins_string(str1, &di, str2, &si, &sc), ==, 0);
        munit_assert_string_equal(str1, str2);
        munit_assert_int(string_ins_string(str1, &di, str2, &si, &sc), ==, 0);
        munit_assert_string_equal(str1, "HelloHello");
        di = 5;
        sc = 4;
        munit_assert_int(string_ins_string(str1, &di, "BEEP", &si, &sc), ==, 0);
        munit_assert_string_equal(str1, "HelloBEEPHello");
        free(str1);
        free(str2);
        return MUNIT_OK;
};

static MunitResult test_string_copy (const MunitParameter params[],
                                     void * data) {
        char * source = "Hello, world!";
        char * dest = calloc(20, sizeof(char));
        int si = 2;
        int count = 5;
        int di = 0;
        munit_assert_int(string_copy(dest, &di, source, &si, &count), ==, 0);
        munit_assert_string_equal(dest, "llo, ");
        si = 0;
        count = 14;
        munit_assert_int(string_copy(dest, &di, source, &si, &count), ==, 0);
        free(dest);
        return MUNIT_OK;
};

static MunitResult test_string_cat (const MunitParameter params[],
                                    void * data) {
        char * source = ", world!";
        char * dest = calloc(20, sizeof(char));
        munit_assert_int(string_cat(dest, "Hello"), ==, 0);
        munit_assert_int(string_cat(dest, source), ==, 0);
        munit_assert_string_equal(dest, "Hello, world!");
        free(dest);
        return MUNIT_OK;
};

static MunitResult test_int_from_char (const MunitParameter params[],
                                       void * data) {
        int dest;
        char source = '5';
        munit_assert_int(int_from_char(&dest, &source), ==, 0);
        munit_assert_int(dest, ==, 5);
        return MUNIT_OK;
};

static MunitResult test_int_from_string (const MunitParameter params[],
                                         void * data) {
        int dest;
        char * src = "789";
        munit_assert_int(int_from_string(&dest, src), ==, 0);
        munit_assert_int(dest, ==, 789);
        src = "-654";
        munit_assert_int(int_from_string(&dest, src), ==, 0);
        munit_assert_int(dest, ==, -654);
        return MUNIT_OK;
};

static MunitResult test_string_from_int (const MunitParameter params[],
                                         void * data) {
        char * dest = calloc(4, sizeof(char));
        int src = 365;
        munit_assert_int(string_from_int(dest, &src), ==, 0);
        munit_assert_string_equal(dest, "365");
        dest = realloc(dest, 7 * sizeof(char));
        dest[0] = LBF_NULL;
        src = -12345;
        munit_assert_int(string_from_int(dest, &src), ==, 0);
        munit_assert_string_equal(dest, "-12345");
        free(dest);
        return MUNIT_OK;
};

static MunitTest lb_string_tests[] = {
        {(char*) "/count/vis", test_string_count_vis, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/count/size", test_string_count_size, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/compare/is equal", test_string_is_equal, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/edit/delete", test_string_del, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/edit/ins char", test_string_ins_char, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/edit/ins string", test_string_ins_string, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/edit/copy", test_string_copy, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/edit/cat", test_string_cat, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/convert/int from char", test_int_from_char, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/convert/int from string", test_int_from_string, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/convert/string from int", test_string_from_int, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_string_suite = {(char*) "/lb_string",
        lb_string_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_STRING_H */
