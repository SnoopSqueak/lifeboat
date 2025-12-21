#include "lb_units.h"

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
        return munit_suite_main(&lb_unit_suite, (void*) "µnit", argc, argv);
};
