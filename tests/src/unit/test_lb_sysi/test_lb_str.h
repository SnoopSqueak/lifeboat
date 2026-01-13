#ifndef TEST_LB_STRING_H
#define TEST_LB_STRING_H

#include "munit.h"
#include "lb_sysi/lb_bool.h"
#include "lb_sysi/lb_str.h"

static MunitResult test_str_from_charcount (const MunitParameter params[],
                                            void * data) {
        int count = 5;
        struct lb_str *str;
        munit_assert_int(str_from_charcount(&str, &count), ==, 0);
        munit_assert_not_null(str->chars);
        munit_assert_int(str->size, ==, 5);
        munit_assert_int(str_free(&str), ==, 0);
        munit_assert_null(str);
        count = LBF_MAX_STR_LEN * 2;
        munit_assert_int(str_from_charcount(&str, &count), !=, 0);
        munit_assert_int(str_free(&str), ==, 0);
        munit_assert_null(str);
        return MUNIT_OK;
};

static MunitResult test_str_from_chars (const MunitParameter params[],
                                        void * data) {
        const char *val = "Hello, world!";
        struct lb_str *str;
        munit_assert_int(str_from_chars(&str, val), ==, 0);
        munit_assert_string_equal(str->chars, val);
        munit_assert_int(str_free(&str), ==, 0);
        munit_assert_null(str);
        return MUNIT_OK;
};

static MunitResult test_count_str_chars (const MunitParameter params[],
                                         void * data) {
        const char *val = "Hello, world!";
        int count;
        struct lb_str *str;
        munit_assert_int(str_from_chars(&str, val), ==, 0);
        munit_assert_int(count_str_chars(&count, str), ==, 0);
        munit_assert_int(count, ==, 14);
        munit_assert_int(str_free(&str), ==, 0);
        return MUNIT_OK;
};

static MunitResult test_char_from_str (const MunitParameter params[],
                                       void * data) {
        const char *val = "Hello, world!";
        char c;
        int i = 0;
        struct lb_str *str;
        munit_assert_int(str_from_chars(&str, val), ==, 0);
        munit_assert_int(char_from_str(&c, str, &i), ==, 0);
        munit_assert_char(c, ==, 'H');
        i = 5;
        munit_assert_int(char_from_str(&c, str, &i), ==, 0);
        munit_assert_char(c, ==, ',');
        i = 100;
        munit_assert_int(char_from_str(&c, str, &i), ==, -1);
        munit_assert_char(c, ==, ',');
        munit_assert_int(str_free(&str), ==, 0);
        return MUNIT_OK;
};

static MunitResult test_comp_str_str (const MunitParameter params[],
                                      void * data) {
        const char *val1 = "Hello!", *val2 = "Goodbye.";
        struct lb_str *str1, *str2, *str3;
        int iseql;
        munit_assert_int(str_from_chars(&str1, val1), ==, 0);
        munit_assert_int(str_from_chars(&str2, val2), ==, 0);
        munit_assert_int(str_from_chars(&str3, val1), ==, 0);
        munit_assert_string_not_equal(val1, val2);
        munit_assert_int(comp_str_str(&iseql, str1, str2), ==, 0);
        munit_assert_int(iseql, ==, LB_FALSE);
        munit_assert_string_not_equal(str1->chars, str2->chars);
        munit_assert_string_equal(val1, val1);
        munit_assert_int(comp_str_str(&iseql, str1, str3), ==, 0);
        munit_assert_int(iseql, ==, LB_TRUE);
        munit_assert_string_equal(str1->chars, str3->chars);
        munit_assert_int(str_free(&str1), ==, 0);
        munit_assert_int(str_free(&str2), ==, 0);
        munit_assert_int(str_free(&str3), ==, 0);
        return MUNIT_OK;
};

static MunitResult test_str_ins_str (const MunitParameter params[],
                                     void * data) {
        int acount = 3;
        const int di = 0, si = 0, bcount = 5;
        struct lb_str *str_a, *str_b;
        munit_assert_int(str_from_charcount(&str_a, &acount), ==, 0);
        munit_assert_not_null(str_a->chars);
        munit_assert_int(str_from_chars(&str_b, "Hello"), ==, 0);
        munit_assert_not_null(str_b->chars);
        munit_assert_int(str_ins_str(str_a, &di, str_b, &si, &bcount), ==, 0);
        munit_assert_string_equal(str_a->chars, str_b->chars);
        munit_assert_int(str_free(&str_a), ==, 0);
        munit_assert_int(str_free(&str_b), ==, 0);
        return MUNIT_OK;
};

static MunitResult test_str_cat_char (const MunitParameter params[],
                                      void * data) {
        const char *val = "ABC";
        char c = 'D';
        struct lb_str *str;
        munit_assert_int(str_from_chars(&str, val), ==, 0);
        munit_assert_int(str_cat_char(str, &c), ==, 0);
        c = 'E';
        munit_assert_int(str_cat_char(str, &c), ==, 0);
        c = 'F';
        munit_assert_int(str_cat_char(str, &c), ==, 0);
        munit_assert_string_equal(str->chars, "ABCDEF");
        munit_assert_int(str_free(&str), ==, 0);
        return MUNIT_OK;
};

static MunitResult test_str_cat_str (const MunitParameter params[],
                                     void * data) {
        const char *val1 = "Hello", *val2 = ", world!";
        struct lb_str *str1, *str2;
        munit_assert_int(str_from_chars(&str1, val1), ==, 0);
        munit_assert_int(str_from_chars(&str2, val2), ==, 0);
        munit_assert_string_equal(str1->chars, "Hello");
        munit_assert_int(str_cat_str(str1, str2), ==, 0);
        munit_assert_string_equal(str1->chars, "Hello, world!");
        munit_assert_string_equal(str2->chars, ", world!");
        munit_assert_int(str_free(&str1), ==, 0);
        munit_assert_int(str_free(&str2), ==, 0);
        return MUNIT_OK;
};

static MunitResult test_str_from_int (const MunitParameter params[],
                                      void * data) {
        int src = 365;
        struct lb_str *str;
        munit_assert_int(str_from_int(&str, &src), ==, 0);
        munit_assert_string_equal(str->chars, "365");
        munit_assert_int(str_free(&str), ==, 0);
        src = -12345;
        munit_assert_int(str_from_int(&str, &src), ==, 0);
        munit_assert_string_equal(str->chars, "-12345");
        munit_assert_int(str_free(&str), ==, 0);
        src = 0;
        munit_assert_int(str_from_int(&str, &src), ==, 0);
        munit_assert_string_equal(str->chars, "0");
        munit_assert_int(str_free(&str), ==, 0);
        return MUNIT_OK;
};

static MunitResult test_str_del_charcount (const MunitParameter params[],
                                           void * data) {
        struct lb_str *str;
        const int i = 7, count = 6;
        munit_assert_int(str_from_chars(&str, "Hello, cruel world!"), ==, 0);
        munit_assert_int(str_del_charcount(str, &i, &count), ==, 0);
        munit_assert_string_equal(str->chars, "Hello, world!");
        munit_assert_int(str->size, >=, 14);
        munit_assert_int(str_free(&str), ==, 0);
        return MUNIT_OK;
};

static MunitResult test_str_del_all (const MunitParameter params[],
                                     void * data) {
        struct lb_str *str;
        munit_assert_int(str_from_chars(&str, "Hello, world!"), ==, 0);
        munit_assert_int(str_del_all(str), ==, 0);
        munit_assert_string_equal(str->chars, "");
        munit_assert_int(str->size, >=, 5);
        munit_assert_int(str_free(&str), ==, 0);
        return MUNIT_OK;
};

static MunitTest lb_str_tests[] = {
        {(char*) "/str/from/charcount", test_str_from_charcount, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/str/from/chars", test_str_from_chars, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/count/chars", test_count_str_chars, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/char/from/str", test_char_from_str, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/comp/str/str", test_comp_str_str, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/str/ins/str", test_str_ins_str, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/str/cat/char", test_str_cat_char, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/str/cat/str", test_str_cat_str, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/str/from/int", test_str_from_int, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/str/del/charcount", test_str_del_charcount, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/str/del/all", test_str_del_all, NULL, NULL,
                MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_str_suite = {(char*) "/lb_str", lb_str_tests,
        NULL, 1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_STRING_H */
