// Mike Stowe 2025
// https://snoopsqueak.com

//~ #include "lb_main.h"
#include "lb_string.h"
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
        struct lb_string * str = init_ntstring("Hello, world!");
        printf("Strang: %s\n", str->ntstring);
        struct lb_string * extra = init_ntstring("shitty fucking ");
        put_in_string(str, 7, extra);
        printf("Strang: %s\n", str->ntstring);
        take_from_string(str, 4, 3);
        if (take_from_string(str, 8, 20) == -1) return EXIT_FAILURE;
        printf("Strang: %s\n", str->ntstring);
        free_string(extra);
        free_string(str);
        //~ return lb_run(MAX_INPUT_LENGTH, TERM_ROWS, TERM_COLS);
};
