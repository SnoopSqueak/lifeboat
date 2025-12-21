#ifndef TEST_LB_STRING_H
#define TEST_LB_STRING_H

#include "../munit.h"
#include "../../../src/lb_string.h"

static MunitResult test_initnt (const MunitParameter params[], void * data) {
        return MUNIT_OK;
};

static MunitTest lb_string_tests[] = {
        {(char*) "/init/nt", test_initnt, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_string_suite = {(char*) "/lb_string", lb_string_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_STRING_H */
