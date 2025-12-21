#include "test_lb_string.h"

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
        //~ printf("OK? %i\n", MUNIT_OK);
        //~ return 0;
        return munit_suite_main(&lb_string_suite, (void*) "µnit", argc, argv);
        //~ return munit_suite_main(&lb_string_suite, NULL, argc, argv);
};
