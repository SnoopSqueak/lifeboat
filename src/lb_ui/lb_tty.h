#ifndef LB_TTY_H
#define LB_TTY_H

#include "lb_string.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lb_stream {
        struct lb_string * string;
        FILE * file;
        int maxsize;
};

struct lb_tty {
        int curi;
        struct lb_stream * ins;
        struct lb_stream * outs;
        int nrow;
        int ncol;
};

struct lb_stream * init_stream (FILE * file, int * maxsize);
int free_stream (struct lb_stream ** stream);
struct lb_tty * init_tty (FILE * fin, int * lin, FILE * fout, int * lout, int * nrow, int * ncol);
int free_tty (struct lb_tty ** tty);
int put_to_tty_out (struct lb_tty * tty, struct lb_string * string);
int put_to_tty_in (struct lb_tty * tty, struct lb_string * string);
int take_from_tty_in (struct lb_tty * tty, int count);
int get_tty_in_line (struct lb_tty * tty);
int clear_tty_in (struct lb_tty * tty);
int clear_tty_out (struct lb_tty * tty);

#endif /* LB_TTY_H */
