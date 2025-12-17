#ifndef LB_MAIN_H
#define LB_MAIN_H

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

int lb_run (size_t max_line_length, size_t num_of_lines, size_t num_of_cols);
int lb_par_init (size_t * max_line_length, size_t * num_of_lines, size_t * num_of_cols);
int lb_par_loop ();
int lb_chld_loop ();
int makeline (char * dest, size_t row, char * front, char * mid, char * end);
int lb_view_landing();
void sig_handler (int signo, siginfo_t *info, void *context);
void intr_handler (int code);
void exit_handler (int code);

#endif /* LB_MAIN_H */
