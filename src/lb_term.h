#ifndef LB_TERM_H
#define LB_TERM_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define ONESLIST "0123456789"

struct lb_term {
        FILE * outstr;
        struct termios * attr;
        struct termios * attrcpy;
        size_t top;
        size_t bottom;
};

struct lb_term * make_term (FILE * outdest);
int free_term(struct lb_term * term);
int clear_term(struct lb_term * term);
int print_to_term(struct lb_term * term, char * msg, size_t len);

#endif /* LB_TERM_H */
