#include "lb_main.h"
#include "lb_term.h"

static int chid;
static struct lb_term * term;

int lb_run (size_t max_line_length, size_t num_of_lines, size_t num_of_cols) {
        chid = fork();
        if (chid == -1) return EXIT_FAILURE;
        if (chid == 0) return lb_chld_loop();
        
        lb_par_init(&max_line_length, &num_of_lines, &num_of_cols);
        int res = lb_par_loop();
        if (res != 0) printf("Unexpected error: %s. Exiting " SOFTWARE_NAME ".\n", strerror(errno));
        return res;
}

int lb_par_init (size_t * max_line_length, size_t * num_of_lines, size_t * num_of_cols) {
        struct sigaction usract;
        usract.sa_flags = SA_SIGINFO;
        usract.sa_sigaction = &sig_handler;
        if (sigaction(SIGUSR1, &usract, NULL) == -1) return EXIT_FAILURE;
        
        term = get_term(stdin, *max_line_length, stdout, *max_line_length * *num_of_lines, *num_of_lines, *num_of_cols);
        
        struct sigaction exitact;
        exitact.sa_flags = SA_SIGINFO;
        exitact.sa_handler = &exit_handler;
        if (sigaction(SIGTERM, &exitact, NULL) == -1) return EXIT_FAILURE;
        if (sigaction(SIGSEGV, &exitact, NULL) == -1) return EXIT_FAILURE;
        if (sigaction(SIGABRT, &exitact, NULL) == -1) return EXIT_FAILURE;
        
        struct sigaction intsigact;
        intsigact.sa_handler = &intr_handler;
        if (sigaction(SIGINT, &intsigact, NULL) == -1) return EXIT_FAILURE;
        
        return 0;
}

int lb_par_loop () {
        while (true) {
                if (lb_view_landing() != 0) return -1;
                get_input(term, term->inbuf, term->MAX_INBUF);
                //~ printf("Processing: %s\n", term->inbuf);
        }
        return 0;
}

int lb_chld_loop () {
        int ppid = getppid();
        union sigval sv;
        sv.sival_ptr = "Hello, parent!";
        while (true) {
                sleep(2);
                sigqueue(ppid, SIGUSR1, sv);
        };
        return 0;
}

int makeline(char * dest, size_t row, char * front, char * mid, char * end) {
        size_t flen = 0;
        size_t mlen = 0;
        size_t elen = 0;
        if (front != NULL) flen = strlen(front);
        if (mid != NULL) mlen += strlen(mid);
        if (end != NULL) elen += strlen(end);
        size_t len = flen + mlen + elen;
        if (len > term->colcount) {
                errno = ERANGE;
                return -1;
        }
        size_t i = 0;
        size_t di = (term->colcount+1) * row;
        size_t mspace;
        size_t mpad;
        size_t mi;
        if (front != NULL) {
                while (i < flen) {
                        dest[di+i] = front[i];
                        i++;
                }
        }
        if (mid != NULL) {
                if (end != NULL) {
                        mspace = term->colcount - elen - i;
                } else {
                        mspace = term->colcount - i;
                }
                mpad = (mspace - mlen)/2;
                mi = 0;
                while (mi < mpad) {
                        dest[di+i] = ' ';
                        mi++;
                        i++;
                }
                mi = 0;
                while (mi < mlen) {
                        dest[di+i] = mid[mi];
                        mi++;
                        i++;
                }
                mi = 0;
                while (mi < mpad) {
                        dest[di+i] = ' ';
                        mi++;
                        i++;
                }
        }
        if (end != NULL) {
                while (i < term->colcount) {
                        if (i < term->colcount - elen) {
                                dest[di+i] = ' ';
                        } else {
                                dest[di+i] = end[i - term->colcount + elen];
                        }
                        i++;
                }
        }
        if (len == 0) {
                while (i < term->colcount) {
                        dest[di+i] = ' ';
                        i++;
                }
        }
        if (row == term->rowcount - 1) {
                dest[di+i] = '\0';
        } else {
                dest[di+i] = '\n';
        }
        return 0;
}

int lb_view_landing() {
        int errors = 0;
        clearterm(term);
        char * lines = calloc((term->colcount + 1) * term->rowcount, sizeof(char));
        errors += makeline(lines, 0, NULL, SOFTWARE_NAME " status: offline", "v" SOFTWARE_VERSION);
        //~ errors += makeline(lines, 1, "Hosting @@ room(s) across ## socket(s).", NULL, "Accepted ## active connection(s).");
        errors += makeline(lines, 1, "Hosting @@ room(s) across ## socket(s). Hosting @@ room(s) across ## socket(s). Hosting @@ room(s) across ## socket(s).", NULL, "Accepted ## active connection(s).");
        errors += makeline(lines, 2, "  (## user(s) waiting to connect to you.)", NULL, NULL);
        errors += makeline(lines, 3, "Joined to ## room(s) across ## socket(s).", NULL, NULL);
        errors += makeline(lines, 4, NULL, NULL, NULL);
        errors += makeline(lines, 5, "Type /help to see available commands.", NULL, NULL);
        errors += makeline(lines, 6, NULL, "(notification area)", NULL);
        errors += makeline(lines, 7, term->inbuf, NULL, NULL);
        if (errors == 0) print_lines(term, lines);
        free(lines);
        return errors;
}

void sig_handler (int signo, siginfo_t *info, void *context) {
        if (signo != SIGUSR1) return;
        //~ printf("\nChild said: %s\n", (char *)(info->si_value.sival_ptr));
        //~ printf("\nChild said: %i\n", info->si_value.sival_int);
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
