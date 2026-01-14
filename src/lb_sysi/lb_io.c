#include "lb_io.h"

typedef struct lb_file {
        FILE *file;
        int fileno;
} lb_file;

typedef struct lb_tty_cfg {
        struct termios * tcattr;
} lb_tty_cfg;

int file_init (struct lb_file **dest, FILE *file, int *fileno) {
        *dest = malloc(sizeof(struct lb_file));
        if (*dest == NULL) return -1;
        (*dest)->file = file;
        (*dest)->fileno = *fileno;
        return 0;
};

int file_init_tty_in (struct lb_file **dest) {
        int fileno = STDIN_FILENO;
        return file_init(dest, stdin, &fileno);
};

int file_init_tty_out (struct lb_file **dest) {
        int fileno = STDOUT_FILENO;
        return file_init(dest, stdout, &fileno);
};

int file_put_str (struct lb_file *dest, const struct lb_str *source) {
        int size;
        if (count_str_chars(&size, source) != 0) return -1;
        if (fputs(source->chars, dest->file) == EOF) return -1;
        return 0;
};

int file_get_key (struct lb_kbevent *dest, const struct lb_file *source) {
        char *keybuf = calloc(18, sizeof(char));
        int i, n;
        while (feof(source->file) == 0) {
                n = read(source->fileno, keybuf, sizeof(keybuf));
                if (n != 1 && n != 3) goto cleankeybuf;
                i = 0;
                while (i < n) {
                        dest->is_press = LB_TRUE;
                        // dest->is_press = ~(keybuf[i] & 0x80) ? LB_TRUE : LB_FALSE;
                        if (i + 2 < n && (keybuf[i] & 0x7f) == 0
                                && (keybuf[i + 1] & 0x80) != 0
                                && (keybuf[i + 2] & 0x80) != 0) {
                                dest->code = ((keybuf[i + 1] & 0x7f) << 7) |
                                (keybuf[i + 2] & 0x7f);
                                i += 3;
                        } else {
                                dest->code = (keybuf[i] & 0x7f);
                                i++;
                        };
                };
                if (dest->is_press) {
                        printf("Pressed '%s'\n", keybuf);
                } else {
                        printf("Released '%s'\n", keybuf);
                };
        };
        free(keybuf);
        return 0;
cleankeybuf:
        free(keybuf);
        return -1;
};

int file_free (struct lb_file **dest) {
        if (*dest == NULL) return -1;
        free(*dest);
        *dest = NULL;
        return 0;
};

int tty_cfg_init (struct lb_tty_cfg **dest) {
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
        free(*dest);
        *dest = NULL;
cleantcattr:
        free(tcattr);
        return -1;
};

int tty_cfg_free (struct lb_tty_cfg **dest) {
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
