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
