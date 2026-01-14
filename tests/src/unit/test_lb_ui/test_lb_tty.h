#ifndef TEST_LB_TTY_H
#define TEST_LB_TTY_H

#include "munit.h"
// #include "lb_sysi/lb_str.h"
#include "lb_ui/lb_tty.h"

#include <stdio.h>

static const char * outpath = "lb_testfile.txt";

static MunitResult test_init_tty (const MunitParameter params[], void * data) {
        return MUNIT_SKIP;
};

static MunitResult test_input (const MunitParameter params[], void * data) {
        return MUNIT_SKIP;
};

static void * setup_nonstd_out (const MunitParameter params[], void * data) {
        FILE * outfile = fopen(outpath, "w+");
        if (outfile == NULL) {
                munit_error(strerror(errno));
        };
        return (void *) outfile;
};

static void tear_down_nonstd_out (void * data) {
        if (data != NULL) fclose((FILE *)data);
};

static MunitResult test_output (const MunitParameter params[], void * data) {
        munit_assert_ptr_not_null(data);
        // int maxsize = 64;
        // int rows = 5;
        // int cols = 4;
        // char * tstring = "This is a test!";
        // struct lb_tty * tty = init_tty(stdin, &maxsize, (FILE *)data, &maxsize, &rows, &cols);
        // munit_assert_ptr_not_null(tty);
        // struct lb_string * testput = init_ntstring(tstring);
        // munit_assert_int(put_to_tty_out(tty, testput), ==, 0);
        // munit_assert_string_equal(tty->outs->string->ntstring, tstring);
        // munit_assert_int(tty->outs->string->ntsize-1, ==, 15);
        // free_tty(&tty);
        // munit_assert_ptr_null(tty);
        return MUNIT_SKIP;
};

static MunitTest lb_tty_tests[] = {
        {(char*) "/init/tty", test_init_tty, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/in/put", test_input, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/out/put", test_output, setup_nonstd_out, tear_down_nonstd_out, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_tty_suite = {(char*) "/lb_tty", lb_tty_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_TTY_H */
