// Mike Stowe 2025
// https://snoopsqueak.com

#include "main.h"

int main (int argc, char ** argv) {
        int maxlinelength = MAX_INPUT_LENGTH;
        int nrow = TERM_ROWS;
        int ncol = TERM_COLS;
        return lb_run(&maxlinelength, &nrow, &ncol);
};
