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

int file_put_string (struct lb_file *dest, char *str) {
        if (fputs(str, dest->file) == EOF) return -1;
        return 0;
};

int file_get_key (struct lb_kbevent *dest, struct lb_file *source) {
        char * keybuf = calloc(18, sizeof(char));
        int i, n;
        while (feof(source->file) == 0) {
                n = read(source->fileno, keybuf, sizeof(keybuf));
                if (n != 1 && n != 3) goto cleankeybuf;
                i = 0;
                while (i < n) {
                        dest->is_press = !(keybuf[i] & 0x80);
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
        };
        free(keybuf);
        return 0;
cleankeybuf:
        free(keybuf);
        return -1;
        // int i = 0;
        // strin[0] = LBF_NULL;
        // curi = 0;
        // linei = 0;
        // char c;
        // bool isesc = false;
        // int elen = LBF_ESCSEQ_LEN;
        // char * ebuf = calloc(elen, sizeof(char));
        // int ei;
        // int ilen;
        // while (feof(fin) == 0) {
        //         // Replace with read call...
        //         // parse scancodes to detect keyup...
        //         c = fgetc(fin);
        //         if (c == EOF || ferror(fin) != 0) {
        //                 errno = EIO;
        //                 return -1;
        //         };
        //         if (c == LBF_RETURN || c == LBF_NEWLINE) {
        //                 break;
        //         };
        //         if (c == LBF_ESCAPE) {
        //                 isesc = true;
        //                 ebuf[0] = LBF_NULL;
        //                 ei = 0;
        //         };
        //         if (!isesc) {
        //                 if (i + 1 < linelen - 1) {
        //                         if (curi < i) {
        //                                 if (string_ins_char(strin, &curi, &c)
        //                                         != 0) return -1;
        //                         } else {
        //                                 strin[i] = c;
        //                                 strin[i+1] = LBF_NULL;
        //                         };
        //                         i++;
        //                         // TODO: core dumped, enter after long line...
        //                         if (i >= ncol) {
        //                                 linei++;
        //                         }
        //                         curi++;
        //                 };
        //         } else {
        //                 if (ei >= elen) {
        //                         errno = ERANGE;
        //                         free(ebuf);
        //                         return -1;
        //                 };
        //                 ebuf[ei] = c;
        //                 ebuf[ei+1] = LBF_NULL;
        //                 ei++;
        //                 if (string_is_equal(ebuf, LBF_LEFT)) {
        //                         if (curi > 0) {
        //                                 curi--;
        //                                 if (curi <= linei) {
        //                                         linei = curi;
        //                                 };
        //                         };
        //                         isesc = false;
        //                 } else if (string_is_equal(ebuf, LBF_RIGHT)) {
        //                         if (string_count_size(&ilen, strin) != 0)
        //                                 return -1;
        //                         if (curi < ilen - 1) {
        //                                 curi++;
        //                                 if (curi > linei + ncol) {
        //                                         linei = curi - ncol;
        //                                 };
        //                         };
        //                         isesc = false;
        //                 } else if (string_is_equal(ebuf, LBF_UP)) {
        //                         isesc = false;
        //                 } else if (string_is_equal(ebuf, LBF_DOWN)) {
        //                         isesc = false;
        //                 };
        //         };
        //         if (tty_draw() != 0) return -1;
        // };
        // free(ebuf);
        // if (string_cat(dest, strin) != 0) return -1;
        // return 0;
};

int file_free (struct lb_file **dest) {
        if (*dest == NULL) return -1;
        free(*dest);
        return 0;
};

int tty_cfg_init (struct lb_tty_cfg **dest) {
        int termsize = sizeof(struct termios);
        struct termios *tcattr = malloc(termsize);
        if (tcattr == NULL) return -1;
        if (tcgetattr(STDOUT_FILENO, tcattr) != 0) goto cleantcattr;
        *dest = malloc(sizeof(lb_tty_cfg));
        if (*dest == NULL) goto cleantcattr;
        (*dest)->tcattr = malloc(termsize);
        memcpy((*dest)->tcattr, tcattr, termsize);
        tcattr->c_lflag &= ~(ECHO | ECHONL | ICANON);
        if (tcsetattr(STDOUT_FILENO, TCSADRAIN, tcattr) != 0) goto cleandest;
        free(tcattr);
        return 0;
cleandest:
        free(*dest);
cleantcattr:
        free(tcattr);
        return -1;
};

int tty_cfg_free (struct lb_tty_cfg **dest) {
        if (*dest == NULL) return -1;
        if ((*dest)->tcattr != NULL) free((*dest)->tcattr);
        free(*dest);
        return 0;
};
