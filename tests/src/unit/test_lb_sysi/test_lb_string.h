#ifndef TEST_LB_STRING_H
#define TEST_LB_STRING_H

#include "munit.h"
#include "lb_sysi/lb_string.h"

static MunitResult test_initnt (const MunitParameter params[], void * data) {
        struct lb_string * string = init_ntstring("Hello, world!");
        munit_assert_ptr_not_null(string);
        munit_assert_size(string->ntsize, ==, 14);
        munit_assert_string_equal(string->ntstring, "Hello, world!");
        munit_assert_char(string->ntstring[string->ntsize - 1], ==, '\0');
        munit_assert_string_equal(string->ntstring, "Hello, world!");
        free_string(&string);
        munit_assert_ptr_null(string);
        return MUNIT_OK;
};

static MunitResult test_initut (const MunitParameter params[], void * data) {
        struct lb_string * string = init_utstring("Hello, world!", 13);
        munit_assert_ptr_not_null(string);
        munit_assert_size(string->ntsize, ==, 14);
        munit_assert_string_equal(string->ntstring, "Hello, world!");
        munit_assert_char(string->ntstring[string->ntsize - 1], ==, '\0');
        munit_assert_string_equal(string->ntstring, "Hello, world!");
        free_string(&string);
        munit_assert_ptr_null(string);
        return MUNIT_OK;
};

static MunitResult test_count_vischar (const MunitParameter params[], void * data) {
        // count_vischar();
        return MUNIT_OK;
};

static MunitResult test_count (const MunitParameter params[], void * data) {
        munit_assert_int(count_ntstring("Hello, world!"), ==, 13);
        munit_assert_int(count_ntstring(""), ==, 0);
        return MUNIT_OK;
};

static MunitResult test_grow (const MunitParameter params[], void * data) {
        struct lb_string * string = init_ntstring("Hello, world!");
        munit_assert_ptr_not_null(string);
        munit_assert_size(string->ntsize, ==, 14);
        munit_assert_int(grow_string(string, 20), ==, 0);
        munit_assert_size(string->ntsize, ==, 20);
        munit_assert_string_equal(string->ntstring, "Hello, world!      ");
        munit_assert_int(grow_string(string, 20), ==, 0);
        munit_assert_size(string->ntsize, ==, 20);
        munit_assert_int(grow_string(string, 10), ==, -1);
        munit_assert_size(string->ntsize, ==, 20);
        munit_assert_string_equal(string->ntstring, "Hello, world!      ");
        free_string(&string);
        munit_assert_ptr_null(string);
        return MUNIT_OK;
};

static MunitResult test_shrink (const MunitParameter params[], void * data) {
        struct lb_string * string = init_ntstring("Hello, world!");
        munit_assert_ptr_not_null(string);
        munit_assert_size(string->ntsize, ==, 14);
        munit_assert_int(shrink_string(string, 10), ==, 0);
        munit_assert_size(string->ntsize, ==, 10);
        munit_assert_string_equal(string->ntstring, "Hello, wo");
        munit_assert_int(shrink_string(string, 10), ==, 0);
        munit_assert_size(string->ntsize, ==, 10);
        munit_assert_int(shrink_string(string, 12), ==, -1);
        munit_assert_size(string->ntsize, ==, 10);
        munit_assert_string_equal(string->ntstring, "Hello, wo");
        free_string(&string);
        munit_assert_ptr_null(string);
        return MUNIT_OK;
};

static MunitResult test_insert (const MunitParameter params[], void * data) {
        struct lb_string * dest = init_ntstring("Hello, world!");
        munit_assert_ptr_not_null(dest);
        struct lb_string * src = init_ntstring(" lousy");
        munit_assert_ptr_not_null(src);
        munit_assert_int(put_in_string(dest, 6, src), ==, 0);
        munit_assert_string_equal(dest->ntstring, "Hello, lousy world!");
        munit_assert_int(put_in_string(dest, -1, src), ==, 0);
        munit_assert_string_equal(dest->ntstring, "Hello, lousy world! lousy");
        free_string(&dest);
        free_string(&src);
        munit_assert_ptr_null(dest);
        munit_assert_ptr_null(src);
        return MUNIT_OK;
};

static MunitResult test_delete (const MunitParameter params[], void * data) {
        struct lb_string * string = init_ntstring("Hello, world!");
        munit_assert_ptr_not_null(string);
        munit_assert_int(take_from_string(string, 0, 5), ==, 0);
        munit_assert_string_equal(string->ntstring, ", world!");
        munit_assert_int(take_from_string(string, 0, -1), ==, 0);
        munit_assert_string_equal(string->ntstring, "");
        munit_assert_int(string->ntsize, ==, 1);
        //~ munit_assert_int(take_from_string(string, 0, -1), ==, 0);
        //~ munit_assert_int(take_from_string(string, 1, -1), ==, -1);
        //~ munit_assert_int(take_from_string(string, 0, 1), ==, -1);
        munit_assert_int(take_from_string(string, 0, 0), ==, 0);
        free_string(&string);
        munit_assert_ptr_null(string);
        return MUNIT_OK;
};

static MunitResult test_clear (const MunitParameter params[], void * data) {
        struct lb_string * string = init_ntstring("Hello, world!");
        munit_assert_ptr_not_null(string);
        munit_assert_int(clear_string(string), ==, 0);
        munit_assert_string_equal(string->ntstring, "");
        munit_assert_int(string->ntsize, ==, 1);
        munit_assert_int(clear_string(string), ==, 0);
        munit_assert_string_equal(string->ntstring, "");
        munit_assert_int(string->ntsize, ==, 1);
        free_string(&string);
        munit_assert_ptr_null(string);
        return MUNIT_OK;
};

static MunitResult test_reverse (const MunitParameter params[], void * data) {
        struct lb_string * string = init_ntstring("Hello, world!");
        munit_assert_ptr_not_null(string);
        munit_assert_int(reverse_string(string), ==, 0);
        munit_assert_string_equal(string->ntstring, "!dlrow ,olleH");
        munit_assert_int(string->ntsize, ==, 14);
        munit_assert_int(reverse_string(string), ==, 0);
        munit_assert_string_equal(string->ntstring, "Hello, world!");
        munit_assert_int(string->ntsize, ==, 14);
        free_string(&string);
        munit_assert_ptr_null(string);
        
        string = init_ntstring("Hello, world");
        munit_assert_ptr_not_null(string);
        munit_assert_int(reverse_string(string), ==, 0);
        munit_assert_string_equal(string->ntstring, "dlrow ,olleH");
        munit_assert_int(string->ntsize, ==, 13);
        munit_assert_int(reverse_string(string), ==, 0);
        munit_assert_string_equal(string->ntstring, "Hello, world");
        munit_assert_int(string->ntsize, ==, 13);
        free_string(&string);
        munit_assert_ptr_null(string);
        return MUNIT_OK;
};

static MunitResult test_char_to_int (const MunitParameter params[], void * data) {
        int * result = malloc(sizeof(int));
        munit_assert_int(char_to_int('5', result), ==, 0);
        munit_assert_int(*result, ==, 5);
        free(result);
        return MUNIT_OK;
};

static MunitResult test_string_to_int (const MunitParameter params[], void * data) {
        struct lb_string * string = init_ntstring("365");
        munit_assert_ptr_not_null(string);
        int * result = malloc(sizeof(int));
        munit_assert_int(string_to_int(string, result), ==, 0);
        munit_assert_int(*result, ==, 365);
        free_string(&string);
        munit_assert_ptr_null(string);
        free(result);
        return MUNIT_OK;
};

static MunitTest lb_string_tests[] = {
        {(char*) "/init/null terminated", test_initnt, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/init/unterminated", test_initut, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/count_vischar", test_count_vischar, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/count", test_count, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/resize/grow", test_grow, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/resize/shrink", test_shrink, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/modify/insert", test_insert, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/modify/delete", test_delete, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/modify/clear", test_clear, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/modify/reverse", test_reverse, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/convert/char to int", test_char_to_int, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/convert/string to int", test_string_to_int, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_string_suite = {(char*) "/lb_string", lb_string_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_STRING_H */
