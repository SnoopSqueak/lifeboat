#ifndef TEST_LB_UNITS_H
#define TEST_LB_UNITS_H

#include "munit.h"
#include "unit/test_lb_string.h"
#include "unit/test_lb_tty.h"

static MunitSuite lb_unit_suites[] = {
        lb_string_suite,
        lb_tty_suite,
        { NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE }
};

static const MunitSuite lb_unit_suite = {(char*) "/unit", NULL, lb_unit_suites, 1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_UNITS_H */
