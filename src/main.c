// Mike Stowe 2025
// https://snoopsqueak.com

#include "lb_main.h"
//~ #include "lb_string.h"
//~ #include <string.h>

#define MAX_INPUT_LENGTH 4096
#define MIN_INPUT_LENGTH 2
//#define MAX_IN_CONS 64
#define MAX_IN_CONS 4
//#define MAX_OUT_CONS 64
#define MAX_OUT_CONS 4
#define DEF_PORT 25252
#define TERM_ROWS 8
#define TERM_COLS 80

int main (int argc, char ** argv) {
        int maxlinelength = MAX_INPUT_LENGTH;
        int nrow = TERM_ROWS;
        int ncol = TERM_COLS;
        return lb_run(&maxlinelength, &nrow, &ncol);
};
