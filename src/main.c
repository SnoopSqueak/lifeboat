// Mike Stowe 2025
// https://snoopsqueak.com

#include "conlist.h"
#include "lb_term.h"
#include "lb_ui.h"

#include <poll.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

#define SOFTWARE_NAME "LifeBoat"

#define MAX_INPUT_LENGTH 4096
#define MIN_INPUT_LENGTH 2
//#define MAX_IN_CONS 64
#define MAX_IN_CONS 4
//#define MAX_OUT_CONS 64
#define MAX_OUT_CONS 4
#define DEF_PORT 25252
#define TERM_ROWS 8

// TODO: move to separate program wrapper file.
struct exit_obj {
        struct lb_term * term;
};

void int_handler (int code) {
        exit(EXIT_SUCCESS);
}

void exit_handler (int code, void * arg) {
        struct exit_obj * eo = arg;
        if (eo != NULL && eo->term != NULL) {
                print_to_term(eo->term, "\n", 2);
                free_term(eo->term);
        }
}

int main (int argc, char ** argv) {
        struct exit_obj * lb_exit_obj = malloc(sizeof(struct exit_obj));
        struct lb_term * term = make_term(stdout);
        lb_exit_obj->term = term;
        if (on_exit(&exit_handler, lb_exit_obj) != 0) return EXIT_FAILURE;
        if (signal(SIGINT, &int_handler) == SIG_ERR) return EXIT_FAILURE;
        clear_term(term);
        print_to_term(term, "This is a test.", 15);
        //~ int sres;
        //~ char * usr_buf = calloc(MAX_INPUT_LENGTH, sizeof(char));
        //~ size_t usri = 0;
        //~ char uchar;
        //~ struct timeval * usr_poll_rate = malloc(sizeof(struct timeval));
        //~ usr_poll_rate->tv_sec = 0;
        //~ usr_poll_rate->tv_usec = 500000; // microseconds, millionths of a second
        //~ fd_set infiles;
        //~ FD_ZERO(&infiles);
        //~ FD_SET(STDIN_FILENO, &infiles);
        //~ while(false) {
                //~ clear_term(term);
                //~ move_cursor_top(term);
                //~ print_to_term(term, "Waiting.", 8);
                //~ move_cursor_bottom(term);
                //~ if (print_to_term(term, usr_buf, usri) == -1) return EXIT_FAILURE;
                //~ sres = select(STDIN_FILENO+1, &infiles, NULL, NULL, usr_poll_rate);
                //~ if (sres == -1) {
                        //~ return EXIT_FAILURE;
                //~ } else if (sres > 0) {
                        //~ while (read(STDIN_FILENO, &uchar, 1) > 0) {
                                //~ usr_buf[usri] = uchar;
                                //~ usri++;
                                //~ if (usri >= MAX_INPUT_LENGTH) {
                                        //~ print_to_term(term, "Input overflow.", 16);
                                        //~ return EXIT_FAILURE;
                                //~ }
                        //~ }
                //~ }
        //~ };
        //~ print_to_term(term, "XWaiting.", 9);
        return EXIT_SUCCESS;
}
