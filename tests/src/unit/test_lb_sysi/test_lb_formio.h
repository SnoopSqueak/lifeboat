#ifndef TEST_LB_FORMIO_H
#define TEST_LB_FORMIO_H

#include "munit.h"
#include "lb_sysi/lb_formio.h"

static MunitResult test_placeholder (const MunitParameter params[], void * data) {
        return MUNIT_OK;
};

static MunitTest lb_formio_tests[] = {
        {(char*) "/placeholder", test_placeholder, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_formio_suite = {(char*) "/lb_formio", lb_formio_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_FORMIO_H */
