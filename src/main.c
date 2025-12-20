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
        //~ struct lb_string * str1 = init_ntstring("This is a test.");
        //~ struct lb_string * str2 = init_ntstring("AAHHH");
        //~ printf("String val: |%s| length %lu\n", str1->ntstring, str1->ntsize);
        //~ put_in_string(str1, 5, str2);
        //~ printf("String val: |%s| length %lu\n", str1->ntstring, str1->ntsize);
        //~ put_in_string(str1, -1, str2);
        //~ printf("String val: |%s| length %lu\n", str1->ntstring, str1->ntsize);
        size_t maxlinelength = MAX_INPUT_LENGTH;
        size_t nrow = TERM_ROWS;
        size_t ncol = TERM_COLS;
        return lb_run(&maxlinelength, &nrow, &ncol);
};
