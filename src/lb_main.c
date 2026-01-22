#include "lb_main.h"

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct lb_cmd {
        const char **names;
        const int (*handler) (char **argv);
} lb_cmd;

void lb_exit (int exit_status) {
        if (tty_free() != 0) {
                system("reset");
                printf("Failed to free tty. Used reset instead.\n");
        };
        if (exit_status == EXIT_FAILURE) {
                printf("Unexpected failure. %s\n", strerror(errno));
        };
        exit(exit_status);
};

void sig_quit (int signal) {
        lb_exit(EXIT_SUCCESS);
};

void sig_err (int signal) {
        lb_exit(EXIT_FAILURE);
};

int handle_quit (char **argv) {
        lb_exit(EXIT_SUCCESS);
        return 0;
};

int get_user_input () {
        char c;
        while (true) {
                if (tty_get_in(&c) != 0) return -1;
                printf("'%c'\n", c);
        };
        return 0;
};

int show_user_output (const int *ncol, const int *nrow, const char *swname,
                      const char *swvers) {
        while (true) {
                sleep(1);
                //if (view_landing(ncol, nrow, SOFTWARE_NAME, SOFTWARE_VERSION) != 0)
                        //return -1;
                printf("Parent.\n");
        };
        return 0;
};

struct lb_cmd cmd_quit = {(const char *[]){"close", "quit"}, &handle_quit};

void lb_main (int *ncol, int *nrow, int *maxlinelength) {
        if (signal(SIGINT, sig_quit) != 0) goto exiterr;
        if (signal(SIGSEGV, sig_err) != 0) goto exiterr;
        if (signal(SIGABRT, sig_quit) != 0) goto exiterr;
        if (signal(SIGTERM, sig_err) != 0) goto exiterr;
        if (signal(SIGILL, sig_err) != 0) goto exiterr;
        if (init_lbthreads() != 0) goto exiterr;
        if (tty_init(ncol, nrow, maxlinelength) != 0) goto exiterr;
        unsigned int chid;
        if (init_thr(&chid, get_user_input) != 0) goto exiterr;
        if (show_user_output(ncol, nrow, SOFTWARE_NAME, SOFTWARE_VERSION) != 0)
                goto exiterr;
        lb_exit(EXIT_SUCCESS);
exiterr:
        lb_exit(EXIT_FAILURE);
};
