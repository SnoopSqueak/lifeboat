#ifndef LB_TTY_H
#define LB_TTY_H

#include <errno.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lb_sysi/lb_fmt.h"
#include "lb_sysi/lb_io.h"
#include "lb_sysi/lb_str.h"

int tty_init (int *ncol, int *nrow, int *maxlinelen);
int tty_draw ();
int tty_put_out (const struct lb_str *source);
int tty_get_in (struct lb_str *dest);
int tty_free ();

#endif /* LB_TTY_H */
