#include "lb_main.h"

#include <stdlib.h>
#include <threads.h>
#include <time.h>

// static struct lb_state * state = NULL;

// int chldloop () {
//         struct timespec tsc;
//         tsc.tv_sec = 2;
//         while (true) {
//                 printf("SCREAMING CHILD!!!\n");
//                 thrd_sleep(&tsc, NULL);
//         };
//         return 0;
// };
//
// int parloop () {
//         struct timespec ts;
//         ts.tv_sec = 2;
//         while (true) {
//                 printf("I'm the parent, blahblahblah.\n");
//                 thrd_sleep(&ts, NULL);
//         };
//         return 0;
// };

int lb_exit (int exit_status) {
        if (tty_free() != 0) {
                system("reset");
                printf("Failed to free tty. Used reset instead.\n");
                return EXIT_FAILURE;
        };
        return exit_status;
};

void signal_handler (int signal) {
        lb_exit(EXIT_FAILURE);
        raise(signal);
};

int lb_main (int *ncol, int *nrow, int *maxlinelength) {
        if (signal(SIGINT, signal_handler) != 0) return -1;
        if (signal(SIGSEGV, signal_handler) != 0) return -1;
        if (signal(SIGABRT, signal_handler) != 0) return -1;
        if (signal(SIGTERM, signal_handler) != 0) return -1;
        if (tty_init(ncol, nrow, maxlinelength) != 0) return -1;
        if (view_landing(ncol, nrow, SOFTWARE_NAME, SOFTWARE_VERSION) != 0) return -1;
        char *usrbuf = calloc(*maxlinelength, sizeof(char));
        if (tty_get_in(usrbuf) != 0) {
                return lb_exit(EXIT_FAILURE);
        };
        if (tty_free() != 0) return EXIT_FAILURE;
        printf("\nUsr input: '%s'.\n", usrbuf);
        free(usrbuf);
        return 0;

        // if (init_lbthreads() != 0) return -1;
        // int chid;
        // if (lb_fork(&chid, chldloop) != 0) return -1;
        // parloop();
        // printf("\nlater.\n");

        // state = malloc(sizeof(struct lb_state));
        // state->chid = 0;
        // state->swname = SOFTWARE_NAME;
        // state->swvers = SOFTWARE_VERSION;
        // int maxoutlength = *maxlinelength * *nrow;
        // state->tty = init_tty(stdin, maxlinelength, stdout, &maxoutlength, nrow, ncol);
        //return lb_exit(EXIT_SUCCESS);
};
