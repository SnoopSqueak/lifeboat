#ifndef TEST_LB_VIEW_H
#define TEST_LB_VIEW_H

#include "munit.h"
#include "lb_ui/lb_view.h"

#include <stdio.h>

static MunitResult test_wip (const MunitParameter params[], void * data) {
        return MUNIT_SKIP;
};

static MunitTest lb_view_tests[] = {
        {(char*) "/wip", test_wip, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_view_suite = {(char*) "/lb_view", lb_view_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_VIEW_H */
