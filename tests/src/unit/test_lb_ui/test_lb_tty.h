#ifndef TEST_LB_TTY_H
#define TEST_LB_TTY_H

#include "munit.h"
#include "lb_sysi/lb_string.h"
#include "lb_ui/lb_tty.h"

#include <stdio.h>

const char * outpath = "testout.txt";

static MunitResult test_initstream (const MunitParameter params[], void * data) {
        int maxsize = 64;
        struct lb_stream * stream = init_stream(stdin, &maxsize);
        munit_assert_ptr_not_null(stream);
        munit_assert_ptr_equal(stream->file, stdin);
        munit_assert_int(stream->maxsize, ==, maxsize);
        munit_assert_string_equal(stream->string->ntstring, "\0");        
        free_stream(&stream);
        munit_assert_ptr_null(stream);
        return MUNIT_OK;
};

static MunitResult test_inittty (const MunitParameter params[], void * data) {
        int maxsize = 64;
        int rows = 5;
        int cols = 4;
        struct lb_tty * tty = init_tty(stdin, &maxsize, stdout, &maxsize, &rows, &cols);
        munit_assert_ptr_not_null(tty);
        munit_assert_ptr_not_null(tty->ins);
        munit_assert_ptr_not_null(tty->outs);
        munit_assert_int(tty->curi, ==, 0);
        munit_assert_int(tty->nrow, ==, rows);
        munit_assert_int(tty->ncol, ==, cols);
        munit_assert_ptr_equal(tty->ins->file, stdin);
        munit_assert_int(tty->ins->maxsize, ==, maxsize);
        munit_assert_string_equal(tty->ins->string->ntstring, "\0");
        munit_assert_ptr_equal(tty->outs->file, stdout);
        munit_assert_int(tty->outs->maxsize, ==, maxsize);
        munit_assert_string_equal(tty->outs->string->ntstring, "\0");
        free_tty(&tty);
        munit_assert_ptr_null(tty);
        return MUNIT_OK;
};

static MunitResult test_input (const MunitParameter params[], void * data) {
        int maxsize = 64;
        int rows = 5;
        int cols = 4;
        char * tstring = "This is a test!";
        struct lb_tty * tty = init_tty(stdin, &maxsize, stdout, &maxsize, &rows, &cols);
        munit_assert_ptr_not_null(tty);
        struct lb_string * testput = init_ntstring(tstring);
        munit_assert_int(put_to_tty_in(tty, testput), ==, 0);
        munit_assert_string_equal(tty->ins->string->ntstring, tstring);
        munit_assert_int(tty->ins->string->ntsize-1, ==, tty->curi);
        free_tty(&tty);
        munit_assert_ptr_null(tty);
        return MUNIT_OK;
};

static void * setup_nonstd_out (const MunitParameter params[], void * data) {
        FILE * file = fopen(outpath, "r+");
        return (void *) file;
};

static void tear_down_nonstd_out (void * data) {
        fclose(data);
};

static MunitResult test_output (const MunitParameter params[], void * data) {
        int maxsize = 64;
        int rows = 5;
        int cols = 4;
        char * tstring = "This is a test!";
        struct lb_tty * tty = init_tty(stdin, &maxsize, (FILE *)data, &maxsize, &rows, &cols);
        munit_assert_ptr_not_null(tty);
        struct lb_string * testput = init_ntstring(tstring);
        munit_assert_int(put_to_tty_out(tty, testput), ==, 0);
        munit_assert_string_equal(tty->outs->string->ntstring, tstring);
        munit_assert_int(tty->outs->string->ntsize-1, ==, 15);
        free_tty(&tty);
        munit_assert_ptr_null(tty);
        return MUNIT_OK;
};

static MunitResult test_intake (const MunitParameter params[], void * data) {
        int maxsize = 64;
        int rows = 5;
        int cols = 4;
        int take_count = 8;
        char * tstring = "This is a test!";
        struct lb_tty * tty = init_tty(stdin, &maxsize, stdout, &maxsize, &rows, &cols);
        munit_assert_ptr_not_null(tty);
        struct lb_string * testput = init_ntstring(tstring);
        munit_assert_int(put_to_tty_in(tty, testput), ==, 0);
        munit_assert_int(take_from_tty_in(tty, take_count), ==, 0);
        munit_assert_string_equal(tty->ins->string->ntstring, "This is");
        munit_assert_int(take_from_tty_in(tty, -1), ==, 0);
        munit_assert_string_equal(tty->ins->string->ntstring, "");
        munit_assert_int(tty->ins->string->ntsize, ==, 1);
        free_tty(&tty);
        munit_assert_ptr_null(tty);
        return MUNIT_OK;
};

//~ static MunitResult test_inget (const MunitParameter params[], void * data) {
        //~ int maxsize = 64;
        //~ int rows = 5;
        //~ int cols = 4;
        //~ int take_count = 8;
        //~ char * tstring = "This is a test!";
        //~ struct lb_tty * tty = init_tty(stdin, &maxsize, stdout, &maxsize, &rows, &cols);
        //~ munit_assert_ptr_not_null(tty);
        //~ struct lb_string * testput = init_ntstring(tstring);
        //~ munit_assert_int(put_to_tty_in(tty, testput), ==, 0);
        //~ munit_assert_string_equal(tty->ins->string->ntstring, tstring);
        // multithread to hit "Enter" after a second?
        //~ munit_assert_int(get_tty_in_line(tty), ==, 0);
        
        //~ munit_assert_int(take_from_tty_in(tty, -1), ==, 0);
        //~ munit_assert_string_equal(tty->ins->string->ntstring, "");
        //~ munit_assert_int(tty->ins->string->ntsize, ==, 1);
        
        //~ free_tty(&tty);
        //~ munit_assert_ptr_null(tty);
        //~ return MUNIT_OK;
//~ };

static MunitResult test_inclear (const MunitParameter params[], void * data) {
        int maxsize = 64;
        int rows = 5;
        int cols = 4;
        char * tstring = "This is a test!";
        struct lb_tty * tty = init_tty((FILE *)data, &maxsize, stdout, &maxsize, &rows, &cols);
        munit_assert_ptr_not_null(tty);
        struct lb_string * testput = init_ntstring(tstring);
        munit_assert_int(put_to_tty_in(tty, testput), ==, 0);
        munit_assert_string_equal(tty->ins->string->ntstring, tstring);
        munit_assert_int(tty->ins->string->ntsize-1, ==, 15);
        munit_assert_int(clear_tty_in(tty), ==, 0);
        munit_assert_string_equal(tty->ins->string->ntstring, "");
        munit_assert_int(tty->ins->string->ntsize, ==, 1);
        free_tty(&tty);
        munit_assert_ptr_null(tty);
        return MUNIT_OK;
};

static MunitResult test_outclear (const MunitParameter params[], void * data) {
        int maxsize = 64;
        int rows = 5;
        int cols = 4;
        char * tstring = "This is a test!";
        struct lb_tty * tty = init_tty(stdin, &maxsize, (FILE *)data, &maxsize, &rows, &cols);
        munit_assert_ptr_not_null(tty);
        struct lb_string * testput = init_ntstring(tstring);
        munit_assert_int(put_to_tty_out(tty, testput), ==, 0);
        munit_assert_string_equal(tty->outs->string->ntstring, tstring);
        munit_assert_int(tty->outs->string->ntsize-1, ==, 15);
        munit_assert_int(clear_tty_out(tty), ==, 0);
        munit_assert_string_equal(tty->outs->string->ntstring, "");
        munit_assert_int(tty->outs->string->ntsize, ==, 1);
        free_tty(&tty);
        munit_assert_ptr_null(tty);
        return MUNIT_OK;
};

static MunitTest lb_tty_tests[] = {
        {(char*) "/init/stream", test_initstream, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/init/tty", test_inittty, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/in/put", test_input, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/out/put", test_output, setup_nonstd_out, tear_down_nonstd_out, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/in/take/delete", test_intake, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        //~ {(char*) "/in/get", test_inget, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/in/clear", test_inclear, setup_nonstd_out, tear_down_nonstd_out, MUNIT_TEST_OPTION_NONE, NULL},
        {(char*) "/out/clear", test_outclear, setup_nonstd_out, tear_down_nonstd_out, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_tty_suite = {(char*) "/lb_tty", lb_tty_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_TTY_H */
