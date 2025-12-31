#include "test_lb_formio.h"

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
        return munit_suite_main(&lb_formio_suite, NULL, argc, argv);
};
