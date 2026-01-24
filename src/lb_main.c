#include "lb_main.h"

// #include <stdlib.h>
// #include <unistd.h>

struct lb_str *usrin;
mtx_t usrinmtx;

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
                if (errno == 0) {
                        printf("Unexpected failure, cause unknown.\n");
                } else {
                        printf("Unexpected failure. %s\n", strerror(errno));
                };
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

struct lb_cmd cmds[] = {
        {(const char *[]){"close", "quit", NULL}, &handle_quit},
        {NULL, NULL}
};

int get_user_input () {
        struct lb_str *str;
        const unsigned int defsize = 8;
        if (init_str_charcount(&str, &defsize) != 0) return -1;
        while (LB_TRUE) {
                if (str_del_all(str) != 0) goto unlockmtx;
                if (tty_get_in(str) != 0) goto cleanstr;
                if (mtx_lock(&usrinmtx) != thrd_success) goto cleanstr;
                if (str_del_all(usrin) != 0) goto unlockmtx;
                if (str_cat_str(usrin, str) != 0) goto unlockmtx;
                if (mtx_unlock(&usrinmtx) != thrd_success) goto cleanstr;
        };
        free_str(&str);
        return 0;
unlockmtx:
        mtx_unlock(&usrinmtx);
cleanstr:
        free_str(&str);
        return -1;
};

int handle_user_input () {
        struct lb_str *str;
        if (mtx_lock(&usrinmtx) != thrd_success) return -1;
        if (init_str_chars(&str, usrin->chars) != 0) goto unlockmtx;
        if (mtx_unlock(&usrinmtx) != thrd_success) goto cleanstr;
        struct lb_cmd *cmd = cmds;
        unsigned int i;
        int res;
        char **argv;
        while (cmd->names != NULL) {
                i = 0;
                while (cmd->names[i] != NULL) {
                        if (comp_chars_chars(&res, cmd->names[i], str->chars) != 0) goto cleanstr;
                        if (res == LB_TRUE) {
                                cmd->handler(argv);
                                goto endloop;
                        };
                        i++;
                };
                cmd++;
        };
endloop:
        free_str(&str);
        return 0;
unlockmtx:
        mtx_unlock(&usrinmtx);
cleanstr:
        free_str(&str);
        return -1;
};

int show_user_output (const int *ncol, const int *nrow, const char *swname,
                      const char *swvers) {
        const unsigned int st = 500;
        while (LB_TRUE) {
                if (thr_sleep(&st) != 0) return -1;
                if (handle_user_input() != 0) return -1;
                if (view_landing(ncol, nrow, SOFTWARE_NAME, SOFTWARE_VERSION) != 0)
                        return -1;
        };
        return 0;
};

void lb_main (int *ncol, int *nrow, int *maxlinelength) {
        if (signal(SIGINT, sig_quit) != 0) goto exiterr;
        if (signal(SIGSEGV, sig_err) != 0) goto exiterr;
        if (signal(SIGABRT, sig_quit) != 0) goto exiterr;
        if (signal(SIGTERM, sig_err) != 0) goto exiterr;
        if (signal(SIGILL, sig_err) != 0) goto exiterr;
        if (init_lbthreads() != 0) goto exiterr;
        if (tty_init(ncol, nrow, maxlinelength) != 0) goto exiterr;
        if (init_str_charcount(&usrin, maxlinelength) != 0) goto exiterr;
        unsigned int chid;
        if (init_thr(&chid, get_user_input) != 0) goto cleanstr;
        if (show_user_output(ncol, nrow, SOFTWARE_NAME, SOFTWARE_VERSION) != 0)
                goto cleanstr;
        lb_exit(EXIT_SUCCESS);
cleanstr:
        free_str(&usrin);
exiterr:
        lb_exit(EXIT_FAILURE);
};
