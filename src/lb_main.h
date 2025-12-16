#ifndef LB_MAIN_H
#define LB_MAIN_H

#include <signal.h>
#include <stddef.h>

#define SOFTWARE_NAME "LifeBoat"

struct usrsig {
        size_t code;
        void * val;
        size_t len;
};

int lb_run (size_t max_line_length, size_t num_of_lines);
int lb_par_init (size_t * max_line_length, size_t * num_of_lines);
int lb_par_loop ();
int lb_chld_loop ();
int lb_show_inbuf ();

void sig_handler (int signo, siginfo_t *info, void *context);
void intr_handler (int code);
void exit_handler (int code);

#endif /* LB_MAIN_H */
