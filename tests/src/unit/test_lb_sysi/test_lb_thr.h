#ifndef TEST_LB_THR_H
#define TEST_LB_THR_H

#include "munit.h"
#include "lb_sysi/lb_thr.h"

static MunitResult test_thr_tba (const MunitParameter params[], void * data) {
        return MUNIT_SKIP;
};

static MunitTest lb_thr_tests[] = {
        {(char*) "/tba", test_thr_tba, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_thr_suite = {(char*) "/lb_thr", lb_thr_tests, NULL,
        1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_THR_H */
