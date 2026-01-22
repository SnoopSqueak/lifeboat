#ifndef TEST_LB_UNITS_H
#define TEST_LB_UNITS_H

#include "munit.h"
#include "test_lb_sysi/test_lb_io.h"
#include "test_lb_sysi/test_lb_fmt.h"
#include "test_lb_sysi/test_lb_str.h"
#include "test_lb_sysi/test_lb_thr.h"
#include "test_lb_ui/test_lb_tty.h"
#include "test_lb_ui/test_lb_view.h"

static MunitSuite lb_unit_suites[] = {
        lb_io_suite,
        lb_fmt_suite,
        lb_str_suite,
        lb_thr_suite,
        lb_tty_suite,
        lb_view_suite,
        { NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE }
};

static const MunitSuite lb_unit_suite = {(char*) "/unit", NULL, lb_unit_suites,
        1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_UNITS_H */
