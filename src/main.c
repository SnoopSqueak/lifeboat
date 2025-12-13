// Mike Stowe 2025
// https://snoopsqueak.com

#include "conlist.h"
#include "lb_term.h"
#include "lb_ui.h"

#include <poll.h>
#include <stdlib.h>
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
        int chid;
        char * usrbuf;
};

struct exit_obj lb_exit_obj;

void int_handler (int code) {
        exit(EXIT_SUCCESS);
}

void sig_handler (int signo, siginfo_t *info, void *context) {
        if (signo != SIGUSR1) return;
        printf("\nChild said: %s\n", (char *)(info->si_value.sival_ptr));
        print_to_term(lb_exit_obj.term, lb_exit_obj.usrbuf, MAX_INPUT_LENGTH);
}

void exit_handler (int code, void * arg) {
        struct exit_obj * eo = arg;
        if (eo != NULL && eo->term != NULL) {
                print_to_term(eo->term, "\n", 1);
                free_term(eo->term);
        }
        if (eo->chid != 0) kill(eo->chid, SIGKILL);
}

int output_loop() {
        int ppid = getppid();
        union sigval sv;
        while(true) {
                sleep(5);
                sv.sival_ptr = "Hello, parent!";
                sigqueue(ppid, SIGUSR1, sv);
        };
        return 0;
}

int main (int argc, char ** argv) {
        int chid = fork();
        if (chid == 0) return output_loop();
        struct sigaction usract;
        usract.sa_flags = SA_SIGINFO;
        usract.sa_sigaction = &sig_handler;
        if (sigaction(SIGUSR1, &usract, NULL) == -1) return EXIT_FAILURE;
        struct lb_term * term = get_term(stdin, MAX_INPUT_LENGTH, stdout, MAX_INPUT_LENGTH * TERM_ROWS);
        lb_exit_obj.term = term;
        lb_exit_obj.chid = chid;
        lb_exit_obj.usrbuf = malloc(MAX_INPUT_LENGTH * sizeof(char));
        if (on_exit(exit_handler, &lb_exit_obj) != 0) return EXIT_FAILURE;
        struct sigaction intsigact;
        intsigact.sa_handler = &int_handler;
        if (sigaction(SIGINT, &intsigact, NULL) == -1) return EXIT_FAILURE;
        print_to_term(term, "Hello\n", 6);
        size_t i;
        while (true) {
                get_input(term, lb_exit_obj.usrbuf, MAX_INPUT_LENGTH);
                printf("Processing: %s\n", lb_exit_obj.usrbuf);
                i = 0;
                while (i < MAX_INPUT_LENGTH) {
                        lb_exit_obj.usrbuf[i] = '\0';
                        i++;
                }
        }
        print_to_term(term, "Farewell.\n", 10);
        return EXIT_SUCCESS;
}
