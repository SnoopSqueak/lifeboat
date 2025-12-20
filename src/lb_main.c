#include "lb_main.h"

static struct lb_state * state = NULL;

int lb_run (size_t * maxlinelength, size_t * nrow, size_t * ncol) {
        pid_t chid = fork();
        if (chid == -1) return EXIT_FAILURE;
        if (chid == 0) return lb_chld_loop();
        
        if (lb_par_init(&chid, maxlinelength, nrow, ncol) == -1) return -1;
        int res = lb_par_loop();
        if (res != 0) {
                printf("Unexpected error: %s. Exiting " SOFTWARE_NAME ".\n", strerror(errno));
                exit_handler(SI_USER);
        };
        free_tty(state->tty);
        free(state);
        return res;
};

int lb_par_init (pid_t * chid, size_t * maxlinelength, size_t * nrow, size_t * ncol) {
        state = malloc(sizeof(struct lb_state));
        state->chid = *chid;
        state->swname = SOFTWARE_NAME;
        state->swvers = SOFTWARE_VERSION;
        size_t maxoutlength = *maxlinelength * *nrow;
        state->tty = init_tty(stdin, maxlinelength, stdout, &maxoutlength, nrow, ncol);
        
        struct sigaction exitact;
        exitact.sa_flags = SA_SIGINFO;
        exitact.sa_handler = &exit_handler;
        if (sigaction(SIGSEGV, &exitact, NULL) == -1) return -1;
        if (sigaction(SIGTERM, &exitact, NULL) == -1) return -1;
        if (sigaction(SIGABRT, &exitact, NULL) == -1) return -1;
        if (sigaction(SIGILL, &exitact, NULL) == -1) return -1;
        
        struct sigaction intsigact;
        intsigact.sa_handler = &intr_handler;
        if (sigaction(SIGINT, &intsigact, NULL) == -1) return -1;
        
        struct sigaction usract;
        usract.sa_flags = SA_SIGINFO;
        usract.sa_sigaction = &sig_handler;
        if (sigaction(SIGUSR1, &usract, NULL) == -1) return -1;
        return 0;
};

int lb_par_loop () {
        while (true) {
                if (view_landing(state) != 0) return -1;
                //get_input(tty, term->inbuf, term->MAX_INBUF);
                //~ get_tty_in_line(state->tty);
                //~ printf("Processing: %s\n", term->inbuf);
        }
        return -1;
};

int lb_chld_loop () {
        int ppid = getppid();
        union sigval sv;
        sv.sival_ptr = "Hello, parent!";
        while (true) {
                sleep(2);
                sigqueue(ppid, SIGUSR1, sv);
        };
        return 0;
};

void sig_handler (int signo, siginfo_t *info, void *context) {
        if (signo != SIGUSR1) return;
        //~ printf("\nChild said: %s\n", (char *)(info->si_value.sival_ptr));
        //~ printf("\nChild said: %i\n", info->si_value.sival_int);
};

void intr_handler (int code) {
        exit(EXIT_SUCCESS);
};

void exit_handler (int code) {
        if (state->chid != 0) kill(state->chid, SIGKILL);
};
