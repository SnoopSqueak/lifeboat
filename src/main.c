// Mike Stowe 2025
// https://snoopsqueak.com

#include "conlist.h"
#include "lb_term.h"
#include "lb_ui.h"

#include <poll.h>
#include <signal.h>
#include <string.h>
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
                print_to_term(eo->term, "\n", 1);
                free_term(eo->term);
        }
}

int main (int argc, char ** argv) {
        struct exit_obj * lb_exit_obj = malloc(sizeof(struct exit_obj));
        struct lb_term * term = make_term(stdout);
        lb_exit_obj->term = term;
        if (on_exit(&exit_handler, lb_exit_obj) != 0) return EXIT_FAILURE;
        if (signal(SIGINT, &int_handler) == SIG_ERR) return EXIT_FAILURE;
        print_to_term(term, "Hello\n", 6);
        size_t usri = 0;
        char * usrin = malloc(MAX_INPUT_LENGTH * sizeof(char));
        size_t usrpi = 0;
        char * usrinpart = malloc(MAX_INPUT_LENGTH * sizeof(char));
        int uiplen;
        char curc;
        fd_set ins;
        fd_set outs;
        int sres;
        int nfds = 0;
        if (STDIN_FILENO > nfds) nfds = STDIN_FILENO;
        if (STDOUT_FILENO > nfds) nfds = STDOUT_FILENO;
        nfds += 1;
        while (true) {
                FD_ZERO(&ins);
                FD_SET(STDIN_FILENO, &ins);
                FD_ZERO(&outs);
                FD_SET(STDOUT_FILENO, &outs);
                sleep(1);
                sres = select(nfds, &ins, &outs, NULL, NULL);
                if (sres > 0) {
                        if (FD_ISSET(STDIN_FILENO, &ins) != 0) {
                                uiplen = read(STDIN_FILENO, usrinpart, MAX_INPUT_LENGTH);
                                usrpi = 0;
                                while (usrpi < uiplen) {
                                        usrin[usri] = usrinpart[usrpi];
                                        usri++;
                                        usrpi++;
                                }
                                printf("\nusrpi: %lu\n", usrpi);
                                if (strcmp(&curc, "\n") == 0) {
                                        print_to_term(term, usrin, usri);
                                        print_to_term(term, "\n", 1);
                                        usri = 0;
                                }
                        }
                        if (FD_ISSET(STDOUT_FILENO, &outs) != 0) {
                                print_to_term(term, "Beep.\n", 6);
                                print_to_term(term, usrin, usri);
                        }
                }
        }
        print_to_term(term, "Farewell.\n", 10);
        return EXIT_SUCCESS;
}
