#include "munit.h"
#include "test_lb_units.h"

static MunitResult test_munit_param (const MunitParameter params[], void * data) {
        munit_assert_string_equal(data, "µnit");
        return MUNIT_OK;
};

static MunitTest test_suite_tests[] = {
        {(char*) "/munit/param", test_munit_param, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static MunitSuite other_suites[] = {
        lb_unit_suite,
        { NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE }
};

static const MunitSuite test_suite = {(char*) "/lifeboat", test_suite_tests, other_suites, 1, MUNIT_SUITE_OPTION_NONE};
