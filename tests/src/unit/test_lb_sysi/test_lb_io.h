#ifndef TEST_LB_IO_H
#define TEST_LB_IO_H

#include "munit.h"
#include "lb_sysi/lb_io.h"

static MunitResult test_io_tba (const MunitParameter params[], void * data) {
        return MUNIT_SKIP;
};

static MunitTest lb_io_tests[] = {
        {(char*) "/tba", test_io_tba, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_io_suite = {(char*) "/lb_io", lb_io_tests, NULL,
        1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_IO_H */
