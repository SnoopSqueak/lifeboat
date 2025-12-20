#ifndef LB_TTY_H
#define LB_TTY_H

#include "lb_string.h"

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
#define LB_ORIGIN "\033[1;1H"
#define LB_CLEAR "\033[J"
#define LB_NEWLINE "\033[13"

struct lb_stream {
        struct lb_string * string;
        FILE * file;
        size_t maxsize;
};

struct lb_tty {
        size_t curi;
        struct lb_stream * ins;
        struct lb_stream * outs;
        struct termios * attr;
        struct termios * attrcpy;
        size_t nrow;
        size_t ncol;
};

struct lb_stream * init_stream (FILE * file, size_t * maxsize);
int free_stream (struct lb_stream * stream);
struct lb_tty * init_tty (FILE * fin, size_t * lin, FILE * fout, size_t * lout, size_t * nrow, size_t * ncol);
int free_tty (struct lb_tty * tty);
int clear_tty_in (struct lb_tty * tty);
int clear_tty_out (struct lb_tty * tty);
int put_to_tty_out (struct lb_tty * tty, struct lb_string * string);
int put_to_tty_in (struct lb_tty * tty, struct lb_string * string);
int take_from_tty_in (struct lb_tty * tty, int count);
int get_tty_in_line (struct lb_tty * tty);

#endif /* LB_TTY_H */
