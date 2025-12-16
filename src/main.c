// Mike Stowe 2025
// https://snoopsqueak.com

#include "lb_main.h"

#include <stdlib.h>

#define MAX_INPUT_LENGTH 4096
#define MIN_INPUT_LENGTH 2
//#define MAX_IN_CONS 64
#define MAX_IN_CONS 4
//#define MAX_OUT_CONS 64
#define MAX_OUT_CONS 4
#define DEF_PORT 25252
#define TERM_ROWS 8

int main (int argc, char ** argv) {
        lb_run(MAX_INPUT_LENGTH, TERM_ROWS);
        return EXIT_SUCCESS;
}
