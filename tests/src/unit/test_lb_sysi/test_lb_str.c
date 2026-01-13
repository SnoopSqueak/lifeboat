#include "test_lb_str.h"

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
        return munit_suite_main(&lb_str_suite, NULL, argc, argv);
};
