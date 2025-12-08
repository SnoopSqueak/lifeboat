// Mike Stowe 2025
// https://snoopsqueak.com

#include "lbui.h"
#include "conlist.h"
#include "logger.h"
#include <signal.h>
#include <sys/select.h>
#include <sys/types.h>
#include <termios.h>

#define SOFTWARE_NAME "LifeBoat"

#define MAX_INPUT_LENGTH 4096
#define MIN_INPUT_LENGTH 2
//#define MAX_IN_CONS 64
#define MAX_IN_CONS 4
//#define MAX_OUT_CONS 64
#define MAX_OUT_CONS 4
#define DEF_PORT 25252

// TODO: move to separate program wrapper file.
struct exit_obj {
        struct termios ** termp;
        struct termios ** termbu;
        struct logger ** dest_logger;
};

// "int" as in "interrupt"
void int_handler (int code) {
        exit(EXIT_SUCCESS);
}

void exit_handler (int code, void * arg) {
        struct exit_obj * eo = arg;
        if (eo != NULL) {
                if (eo->termp != NULL && eo->termbu != NULL) {
                        memcpy(*(eo->termp), *(eo->termbu), sizeof(struct termios));
                        if (tcsetattr(STDIN_FILENO, TCSADRAIN, *(eo->termp)) == -1) {
                                printf("Failed to set terminal mode back to original settings. If you can't see this message, I apologize.\n");
                        }
                }
                if (*(eo->dest_logger) != NULL) {
                        log_this(*(eo->dest_logger), "Exit code %i.\n", LL_DEBUG, code);
                }
                printf("termp: %i\n", (*(eo->termp))->c_lflag);
                printf("termbu: %i\n", (*(eo->termbu))->c_lflag);
        } else {
                printf("Warning: No exit object found.\n");
        }
        printf(AC_RESET);
}

int main (int argc, char ** argv) {
        // Initialize a logger.
        struct logger * lb_logger = make_logger(LL_NEVER, stdout, CF_ANSI);
        if (lb_logger == NULL) {
                printf("Failed to create logger.\n");
                return EXIT_FAILURE;
        }
        log_this(lb_logger, "%s main function entered.\n", LL_DEBUG, SOFTWARE_NAME);
        
        // Initialize terminal attribute set pointer and backup.
        size_t term_size = sizeof(struct termios);
        struct termios * termp = malloc(term_size);
        struct termios * termbu = malloc(term_size);
        tcgetattr(STDIN_FILENO, termp);
        memcpy(termbu, termp, term_size);
        // Switch to non-canonical mode so I can output without waiting for newline.
        termp->c_lflag &= ~(ECHO | ECHONL | ICANON);
        if (tcsetattr(STDIN_FILENO, TCSADRAIN, termp) == -1) {
                log_this(lb_logger, "Failed to set terminal mode to non-canonical.\n", LL_ERROR);
                return EXIT_FAILURE;
        }
        log_this(lb_logger, "Switched to non-canonical mode.\n", LL_DEBUG);
        
        // Initialize an exit handler.
        struct exit_obj * lb_exit_obj = malloc(sizeof(struct exit_obj));
        lb_exit_obj->dest_logger = &lb_logger;
        lb_exit_obj->termp = &termp;
        lb_exit_obj->termbu = &termbu;
        // Assign exit handler to run on success, fail, and interrupt.
        if (on_exit(&exit_handler, lb_exit_obj) != 0) {
                log_this(lb_logger, "Failed to set exit handler.\n", LL_ERROR);
                return EXIT_FAILURE;
        }
        if (signal(SIGINT, &int_handler) == SIG_ERR) {
                log_this(lb_logger, "Failed to set signal handler: %s.\n", LL_ERROR, strerror(errno));
                return EXIT_FAILURE;
        }
        log_this(lb_logger, "Assigned exit and signal handlers.\n", LL_DEBUG);
        
        // Initialize host and client connections.
        //~ struct conlist * hostcons = make_conlist(MAX_IN_CONS);
        
        // Prepare for user input.
        log_this(lb_logger, "Welcome to your %s.\n", LL_INFO, SOFTWARE_NAME);
        int sres;
        char usr_buf[MAX_INPUT_LENGTH];
        struct timeval * usr_poll_rate = malloc(sizeof(struct timeval));
        usr_poll_rate->tv_sec = 0;
        // microseconds, millionths of a second
        usr_poll_rate->tv_usec = 5000000;
        fd_set infiles;
        FD_ZERO(&infiles);
        FD_SET(STDIN_FILENO, &infiles);
        // Main program loop.
        while(true) {
                // \x1b[H moves cursor to 1,1
                // \x1b[0;J clears from cursor to N, or to end
                printf("\x1b[H\x1b[0;J");
                // Get user input.
                sres = select(1, &infiles, NULL, NULL, usr_poll_rate);
                if (sres == -1) {
                        log_this(lb_logger, "Error: %s\n", LL_ERROR, strerror(errno));
                        return EXIT_FAILURE;
                } else if (sres > 0) {
                        if (read(STDIN_FILENO, usr_buf, MAX_INPUT_LENGTH) == -1) {
                                log_this(lb_logger, "Error reading standard input: %s\n", LL_WARN, strerror(errno));
                        }
                        // TODO: detect newline, parse input
                        printf("%s\n", usr_buf);
                }
        };
        log_this(lb_logger, "Exiting %s main function.\n", LL_DEBUG, SOFTWARE_NAME);
        return EXIT_SUCCESS;
}
