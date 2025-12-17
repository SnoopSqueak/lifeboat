#ifndef LB_TERM_H
#define LB_TERM_H

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define ONESLIST "0123456789"
#define AS_BACKSPACE 8
#define AS_NEWLINE 10
#define AS_RETURN 13
#define AS_ESCAPE 27
#define AS_A 65
#define AS_B 66
#define AS_C 67
#define AS_D 68
#define AS_OSB 91
#define AS_DELETE 127
#define AS_UP AS_A
#define AS_DOWN AS_B
#define AS_RIGHT AS_C
#define AS_LEFT AS_D

#define LB_UP "\033[1A"
#define LB_DOWN "\033[1B"
#define LB_RIGHT "\033[1C"
#define LB_LEFT "\033[1D"
#define LB_CLEAR "\033[J"
#define LB_NEWLINE "\033[13"

struct lb_term {
        size_t ini;
        char * usrbuf;
        FILE * instr;
        char * inbuf;
        size_t MAX_INBUF;
        FILE * outstr;
        char * outbuf;
        size_t MAX_OUTBUF;
        struct termios * attr;
        struct termios * attrcpy;
        size_t rowcount;
        size_t colcount;
};

struct lb_term * get_term (FILE * instr, size_t inmax, FILE * outdest, size_t outmax, size_t rowcount, size_t colcount);
int clearinbuf (struct lb_term * term);
int get_input (struct lb_term * term, char * dest, size_t len);
int free_term (struct lb_term * term);
int clearterm (struct lb_term * term);
int print_to_term (struct lb_term * term, char * msg, size_t len);
int print_lines (struct lb_term * term, char * lines);

#endif /* LB_TERM_H */
