#ifndef LB_MAIN_H
#define LB_MAIN_H

#include "lb_state.h"
#include "lb_tty.h"
#include "lb_ui.h"

#include <signal.h>
#include <stddef.h>
#include <stdio.h>

#define SOFTWARE_NAME "LifeBoat"
#define SOFTWARE_VERSION "0.0.0"

struct usrsig {
        size_t code;
        void * val;
        size_t len;
};

int lb_run (size_t * max_line_length, size_t * num_of_lines, size_t * num_of_cols);
int lb_par_init (pid_t * chid, size_t * max_line_length, size_t * num_of_lines, size_t * num_of_cols);
int lb_par_loop ();
int lb_chld_loop ();
void sig_handler (int signo, siginfo_t *info, void *context);
void intr_handler (int code);
void exit_handler (int code);

#endif /* LB_MAIN_H */
