#ifndef LB_MAIN_H
#define LB_MAIN_H

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "lb_ui/lb_tty.h"
#include "lb_ui/lb_view.h"

#define SOFTWARE_NAME "LifeBoat"
#define SOFTWARE_VERSION "0.0.0"

void lb_main (int * ncol, int * nrow, int * maxlinelength);

#endif /* LB_MAIN_H */
