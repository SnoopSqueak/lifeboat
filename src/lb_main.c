#include "lb_main.h"

#include <stdlib.h>

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

void lb_main (int *ncol, int *nrow, int *maxlinelength) {
        if (signal(SIGINT, sig_quit) != 0) goto exiterr;
        if (signal(SIGSEGV, sig_err) != 0) goto exiterr;
        if (signal(SIGABRT, sig_err) != 0) goto exiterr;
        if (signal(SIGTERM, sig_err) != 0) goto exiterr;
        if (signal(SIGILL, sig_err) != 0) goto exiterr;
        if (tty_init(ncol, nrow, maxlinelength) != 0) goto exiterr;
        if (view_landing(ncol, nrow, SOFTWARE_NAME, SOFTWARE_VERSION) != 0) goto exiterr;
        char *usrbuf = calloc(*maxlinelength, sizeof(char));
        if (tty_get_in(usrbuf) != 0) goto exiterr;
        printf("\nUsr input: '%s'.\n", usrbuf);
        free(usrbuf);
        lb_exit(EXIT_SUCCESS);
exiterr:
        lb_exit(EXIT_FAILURE);
};
