#ifndef LB_TTY_H
#define LB_TTY_H

#include "lb_sysi/lb_string.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_tty (int * ncol, int * nrow, int * maxlinelen);
int draw_tty ();
int put_tty_out (char * source);
int get_tty_in (char * dest);
int release_tty ();

#endif /* LB_TTY_H */
