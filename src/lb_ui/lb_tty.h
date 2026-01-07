#ifndef LB_TTY_H
#define LB_TTY_H

#include "lb_sysi/lb_string.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tty_init (int * ncol, int * nrow, int * maxlinelen);
int tty_draw ();
int tty_put_out (char * source);
int tty_get_in (char * dest);
int tty_free ();

#endif /* LB_TTY_H */
