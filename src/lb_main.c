#include "lb_main.h"
#include "lb_term.h"

#include <stdio.h> // only for printf

static int chid;
static struct lb_term * term;

int lb_run (size_t max_line_length, size_t num_of_lines) {
        chid = fork();
        if (chid == -1) return EXIT_FAILURE;
        if (chid == 0) return lb_chld_loop();
        
        lb_par_init(&max_line_length, &num_of_lines);
        lb_par_loop();
        
        return 0;
}

int lb_par_init (size_t * max_line_length, size_t * num_of_lines) {
        struct sigaction usract;
        usract.sa_flags = SA_SIGINFO;
        usract.sa_sigaction = &sig_handler;
        if (sigaction(SIGUSR1, &usract, NULL) == -1) return EXIT_FAILURE;
        
        term = get_term(stdin, *max_line_length, stdout, *max_line_length * *num_of_lines, *num_of_lines);
        
        struct sigaction exitact;
        exitact.sa_flags = SA_SIGINFO;
        exitact.sa_handler = &exit_handler;
        if (sigaction(SIGTERM, &exitact, NULL) == -1) return EXIT_SUCCESS;
        
        struct sigaction intsigact;
        intsigact.sa_handler = &intr_handler;
        if (sigaction(SIGINT, &intsigact, NULL) == -1) return EXIT_FAILURE;
        
        return 0;
}

int lb_show_inbuf () {
        movcurbotleft(term);
        print_to_term(term, term->inbuf, term->MAX_INBUF);
        movcurleft(term);
        relmovcur(term, term->ini, 0);
        return 0;
}

int lb_par_loop() {
        while (true) {
                lb_show_inbuf();
                get_input(term, term->inbuf, term->MAX_INBUF);
                //~ printf("Processing: %s\n", term->inbuf);
        }
        return 0;
}

int lb_chld_loop() {
        int ppid = getppid();
        union sigval sv;
        sv.sival_ptr = "Hello, parent!";
        while(true) {
                sleep(2);
                sigqueue(ppid, SIGUSR1, sv);
        };
        return 0;
}

void sig_handler (int signo, siginfo_t *info, void *context) {
        if (signo != SIGUSR1) return;
        clearterm(term);
        movcurtopleft(term);
        printf("\nChild said: %s\n", (char *)(info->si_value.sival_ptr));
        //~ printf("\nChild said: %i\n", info->si_value.sival_int);
        lb_show_inbuf();
}

void intr_handler (int code) {
        exit(EXIT_SUCCESS);
}

void exit_handler (int code) {
        if (term != NULL) {
                print_to_term(term, "\n", 1);
                free_term(term);
        }
        if (chid != 0) kill(chid, SIGKILL);
}
