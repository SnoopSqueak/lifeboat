#include <termios.h>
#include <unistd.h>

#include "lb_io.h"

typedef struct lb_file {
        FILE *file;
        int fileno;
} lb_file;

typedef struct lb_tty_cfg {
        struct termios * tcattr;
} lb_tty_cfg;

int init_file (struct lb_file **dest, FILE *file, int *fileno) {
        *dest = malloc(sizeof(struct lb_file));
        if (*dest == NULL) {
                errno = ENOMEM;
                return -1;
        };
        (*dest)->file = file;
        (*dest)->fileno = *fileno;
        return 0;
};

int init_file_stdin (struct lb_file **dest) {
        int fileno = STDIN_FILENO;
        return init_file(dest, stdin, &fileno);
};

int init_file_stdout (struct lb_file **dest) {
        int fileno = STDOUT_FILENO;
        return init_file(dest, stdout, &fileno);
};

int file_put_clear (struct lb_file *dest) {
        struct lb_str *es;
        if (init_str_chars(&es, "") != 0) return -1;
        if (fmt_clear(es) != 0) goto cleanstr;
        if (fputs(es->chars, dest->file) == EOF) goto cleanstr;
        free_str(&es);
        return 0;
cleanstr:
        free_str(&es);
        return -1;
};

int file_put_str (struct lb_file *dest, const struct lb_str *src) {
        int size;
        if (count_str_chars(&size, src) != 0) return -1;
        if (size < 1) return -1;
        if (fputs(src->chars, dest->file) == EOF) return -1;
        return 0;
};

int file_get_key (char *dest, const struct lb_file *src) {
        read(src->fileno, dest, sizeof(dest));
        return 0;
};

int free_file (struct lb_file **dest) {
        if (*dest == NULL) return -1;
        free(*dest);
        *dest = NULL;
        return 0;
};

int init_tty_cfg (struct lb_tty_cfg **dest) {
        int termsize = sizeof(struct termios);
        struct termios *tcattr = malloc(termsize);
        if (tcattr == NULL) return -1;
        if (tcgetattr(STDOUT_FILENO, tcattr) != 0) goto cleantcattr;
        *dest = malloc(sizeof(struct lb_tty_cfg));
        if (*dest == NULL) goto cleantcattr;
        (*dest)->tcattr = malloc(termsize);
        memcpy((*dest)->tcattr, tcattr, termsize);
        tcattr->c_lflag &= ~(ECHO | ICANON);
        tcattr->c_cc[VMIN] = 1;
        tcattr->c_cc[VTIME] = 0;
        if (tcsetattr(STDOUT_FILENO, TCSADRAIN, tcattr) != 0) goto cleandest;
        free(tcattr);
        return 0;
cleandest:
        free_tty_cfg(dest);
cleantcattr:
        free(tcattr);
        return -1;
};

int free_tty_cfg (struct lb_tty_cfg **dest) {
        if (dest == NULL || *dest == NULL) return -1;
        if ((*dest)->tcattr != NULL) {
                if (tcsetattr(STDIN_FILENO, TCSADRAIN, (*dest)->tcattr) != 0)
                        goto cleandest;
                free((*dest)->tcattr);
        };
        free(*dest);
        *dest = NULL;
        return 0;
cleandest:
        free(*dest);
        *dest = NULL;
        return -1;
};
