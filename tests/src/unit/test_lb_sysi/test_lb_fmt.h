#ifndef TEST_LB_FMT_H
#define TEST_LB_FMT_H

#include "munit.h"
#include "lb_sysi/lb_fmt.h"

static MunitResult test_fmt_tba (const MunitParameter params[], void * data) {
        return MUNIT_SKIP;
};

static MunitTest lb_fmt_tests[] = {
        {(char*) "/tba", test_fmt_tba, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_fmt_suite = {(char*) "/lb_fmt", lb_fmt_tests, NULL,
        1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_FMT_H */
