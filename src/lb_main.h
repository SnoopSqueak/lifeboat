#ifndef LB_MAIN_H
#define LB_MAIN_H

#include "lb_state.h"
#include "lb_ui/lb_tty.h"
#include "lb_ui/lb_view.h"
#include "lb_sysi/lb_threads.h"

#include <stdio.h>

#define SOFTWARE_NAME "LifeBoat"
#define SOFTWARE_VERSION "0.0.0"

int lb_main (int * maxlinelength, int * nrow, int * ncol);

#endif /* LB_MAIN_H */
