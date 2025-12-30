#ifndef TEST_LB_THREADS_H
#define TEST_LB_THREADS_H

#include "munit.h"
#include "lb_sysi/lb_threads.h"

int * tmp;

int give_me_five () {
        *tmp += 5;
        return 5;
};

static MunitResult test_fork (const MunitParameter params[], void * data) {
        int * chid;
        int count = 0;
        *tmp = 0;
        munit_assert_int(*tmp, ==, 0);
        munit_assert_int(lb_fork(chid, give_me_five), ==, 0);
        munit_assert_int(*tmp, ==, 5);
        munit_assert_int(*chid, >, 0);
        munit_assert_int(free_thread(chid), ==, 0);
        return MUNIT_OK;
};

static MunitTest lb_threads_tests[] = {
        {(char*) "/fork", test_fork, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite lb_threads_suite = {(char*) "/lb_threads", lb_threads_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

#endif /* TEST_LB_THREADS_H */
