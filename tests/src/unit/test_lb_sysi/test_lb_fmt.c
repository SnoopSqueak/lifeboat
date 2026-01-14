#include "test_lb_fmt.h"

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
        return munit_suite_main(&lb_fmt_suite, NULL, argc, argv);
};
