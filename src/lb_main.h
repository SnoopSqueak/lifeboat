#ifndef LB_MAIN_H
#define LB_MAIN_H

#include "lb_ui/lb_state.h"
#include "lb_ui/lb_tty.h"
#include "lb_ui/lb_view.h"

#include <signal.h>
#include <stddef.h>
#include <stdio.h>

#define SOFTWARE_NAME "LifeBoat"
#define SOFTWARE_VERSION "0.0.0"

struct usrsig {
        int code;
        void * val;
        int len;
};

int lb_run (int * max_line_length, int * num_of_lines, int * num_of_cols);
int lb_par_init (int * chid, int * max_line_length, int * num_of_lines, int * num_of_cols);
int lb_par_loop ();
int lb_chld_loop ();
//~ void sig_handler (int signo, siginfo_t *info, void *context);
void intr_handler (int code);
void exit_handler (int code);

#endif /* LB_MAIN_H */
