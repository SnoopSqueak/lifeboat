#ifndef TEST_LB_STRING_H
#define TEST_LB_STRING_H

#include "munit.h"
#include "lb_sysi/lb_string.h"

static MunitResult test_count_string_size (const MunitParameter params[], void * data) {
        int result;
        munit_assert_int(count_string_size(&result, "Hello"), ==, 0);
        munit_assert_int(result, ==, 6);
        munit_assert_int(count_string_size(&result, ""), ==, 0);
        munit_assert_int(result, ==, 1);
        char * tstr = LBF_RESET "TEST!";
        munit_assert_int(count_string_size(&result, tstr), ==, 0);
        munit_assert_int(result, ==, 16);
        return MUNIT_OK;
};

static MunitResult test_count_string_vis (const MunitParameter params[], void * data) {
        int result;
        munit_assert_int(count_string_vis(&result, "Hello"), ==, 0);
        munit_assert_int(result, ==, 5);
        munit_assert_int(count_string_vis(&result, ""), ==, 0);
        munit_assert_int(result, ==, 0);
        char * tstr = LBF_RESET "TEST!";
        munit_assert_int(count_string_vis(&result, tstr), ==, 0);
        munit_assert_int(result, ==, 5);
        return MUNIT_OK;
};

static MunitResult test_string_copy (const MunitParameter params[], void * data) {
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

static MunitResult test_string_cat (const MunitParameter params[], void * data) {
        char * source = ", world!";
        char * dest = calloc(20, sizeof(char));
        munit_assert_int(string_cat(dest, "Hello"), ==, 0);
        munit_assert_int(string_cat(dest, source), ==, 0);
        munit_assert_string_equal(dest, "Hello, world!");
        free(dest);
        return MUNIT_OK;
};

static MunitResult test_int_from_char (const MunitParameter params[], void * data) {
        int dest;
        char source = '5';
        munit_assert_int(int_from_char(&dest, &source), ==, 0);
        munit_assert_int(dest, ==, 5);
        return MUNIT_OK;
};

static MunitResult test_int_from_string (const MunitParameter params[], void * data) {
        int dest;
        char * src = "789";
        munit_assert_int(int_from_string(&dest, src), ==, 0);
        munit_assert_int(dest, ==, 789);
        src = "-654";
        munit_assert_int(int_from_string(&dest, src), ==, 0);
        munit_assert_int(dest, ==, -654);
        return MUNIT_OK;
};

static MunitResult test_string_from_int (const MunitParameter params[], void * data) {
        char * dest = calloc(4, sizeof(char));
        int src = 365;
        munit_assert_int(string_from_int(dest, &src), ==, 0);
        munit_assert_string_equal(dest, "365");
        dest = realloc(dest, 7 * sizeof(char));
        dest[0] = '\0';
        src = -12345;
        munit_assert_int(string_from_int(dest, &src), ==, 0);
        munit_assert_string_equal(dest, "-12345");
        free(dest);
        return MUNIT_OK;
};

static MunitTest lb_string_tests[] = {
        {(char*) "/count/string size", test_count_string_size, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/count/string vis", test_count_string_vis, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/copy", test_string_copy, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/cat", test_string_cat, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/convert/int from char", test_int_from_char, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/convert/int from string", test_int_from_string, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/convert/string from int", test_string_from_int, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_string_suite = {(char*) "/lb_string", lb_string_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_STRING_H */
