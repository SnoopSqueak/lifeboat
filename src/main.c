// Mike Stowe 2025
// https://snoopsqueak.com

#include "main.h"

void main (int argc, char ** argv) {
        int ncol = TERM_COLS;
        int nrow = TERM_ROWS;
        int maxlinelength = MAX_INPUT_LENGTH;
        lb_main(&ncol, &nrow, &maxlinelength);
};
