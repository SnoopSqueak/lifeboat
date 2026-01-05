#include "lb_main.h"

#include <threads.h>
#include <time.h>

// static struct lb_state * state = NULL;

int chldloop () {
        struct timespec tsc;
        tsc.tv_sec = 2;
        while (true) {
                printf("SCREAMING CHILD!!!\n");
                thrd_sleep(&tsc, NULL);
        };
        return 0;
};

int parloop () {
        struct timespec ts;
        ts.tv_sec = 2;
        while (true) {
                printf("I'm the parent, blahblahblah.\n");
                thrd_sleep(&ts, NULL);
        };
        return 0;
};

int lb_main (int * ncol, int * nrow, int * maxlinelength) {
        // int maxoutlength = *maxlinelength * *nrow;
        // struct lb_tty * tty = init_tty(stdin, maxlinelength, stdout, &maxoutlength, nrow, ncol);
        init_tty(ncol, nrow, maxlinelength);
        view_landing(ncol, nrow, SOFTWARE_NAME, SOFTWARE_VERSION);
        char * usrbuf = calloc(*maxlinelength, sizeof(char));
        get_tty_in(usrbuf);
        printf("User input: '%s'\n", usrbuf);

        // struct lb_string * test = init_ntstring("Hello!" LBF_FG_GREEN "TEST!" LBF_FG_RESET);
        // printf(test->ntstring);
        // printf("\n");
        // int c = count_vischar(test->ntstring);
        // printf("count: %i\n", c);
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
        return 0;
};
