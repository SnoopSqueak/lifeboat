#include "lb_tty.h"

struct lb_stream * init_stream (FILE * file, int * fileno, int * maxsize) {
        struct lb_stream * stream = malloc(sizeof(struct lb_stream));
        if (stream == NULL) {
                errno = ENOMEM;
                return NULL;
        };
        stream->file = file;
        stream->string = init_ntstring("");
        if (stream->string == NULL) {
                errno = ENOMEM;
                return NULL;
        };
        stream->maxsize = *maxsize;
        return stream;
};

int free_stream (struct lb_stream * stream) {
        if (free_string(&stream->string) == -1) return -1;
        free(stream);
        return 0;
};

struct lb_tty * init_tty (FILE * fin, int * finno, int * lin, FILE * fout, int * foutno, int * lout, int * nrow, int * ncol) {
        size_t termsize = sizeof(struct termios);
        struct lb_tty * tty = malloc(sizeof(struct lb_tty));
        if (tty == NULL) {
                errno = ENOMEM;
                return NULL;
        };
        tty->ins = init_stream(fin, finno, lin);
        tty->outs = init_stream(fout, foutno, lout);
        tty->attr = malloc(termsize);
        tty->attrcpy = malloc(termsize);
        if (tty->ins == NULL || tty->outs == NULL || tty->attr == NULL || tty->attrcpy == NULL) {
                errno = ENOMEM;
                return NULL;
        };
        tty->ins->maxsize = *lin;
        tty->outs->maxsize = *lout;
        tty->curi = 0;
        tty->nrow = *nrow;
        tty->ncol = *ncol;
        setvbuf(fout, NULL, _IONBF, BUFSIZ);
        tcgetattr(*foutno, tty->attr);
        memcpy(tty->attrcpy, tty->attr, termsize);
        tty->attr->c_lflag &= (ECHO | ISIG);
        tty->attr->c_lflag &= ~(ICANON);
        //~ tty->attr->c_lflag &= (ISIG);
        //~ tty->attr->c_lflag &= ~(ECHO | ICANON);
        tcsetattr(*foutno, TCSADRAIN, tty->attr);
        return tty;
};

int free_tty (struct lb_tty * tty) {
        bool tripped = false;
        tcsetattr(tty->outs->fileno, TCSADRAIN, tty->attrcpy);
        if (free_stream(tty->ins) == -1) tripped = true;
        if (free_stream(tty->outs) == -1) tripped = true;
        free(tty->attr);
        free(tty->attrcpy);
        free(tty);
        return tripped ? -1 : 0;
};

int clear_tty_in (struct lb_tty * tty) {
        if (take_from_string(tty->ins->string, 0, -1) == -1) return -1;
        tty->curi = 0;
        return 0;
};

int clear_tty_out (struct lb_tty * tty) {
        struct lb_string * orig = init_ntstring(AC_ORIGIN);
        if (orig == NULL) {
                errno = ENOMEM;
                return -1;
        };
        if (put_to_tty_out(tty, orig) == -1) return -1;
        if (free_string(&orig) == -1) return -1;
        struct lb_string * clear = init_ntstring(AC_CLEAR);
        if (clear == NULL) {
                errno = ENOMEM;
                return -1;
        };
        if (put_to_tty_out(tty, clear) == -1) return -1;
        if (free_string(&clear) == -1) return -1;
        if (clear_string(tty->outs->string) == -1) return -1;
        return 0;
};

int put_to_tty_out (struct lb_tty * tty, struct lb_string * string) {
        if (put_in_string(tty->outs->string, -1, string) == -1) return -1;
        write(tty->outs->fileno, tty->outs->string->ntstring, tty->outs->string->ntsize);
        return 0;
};

int put_to_tty_in (struct lb_tty * tty, struct lb_string * string) {
        if (put_in_string(tty->ins->string, tty->curi, string) == -1) return -1;
        tty->curi += string->ntsize - 1;
        return 0;
};

int take_from_tty_in (struct lb_tty * tty, int count) {
        if (count == 0) return 0;
        int i = tty->curi;
        if (count < 0) {
                i -= count;
                if (i < 0) {
                        count += i;
                        i = 0;
                };
                tty->curi -= count;
        };
        if (take_from_string(tty->ins->string, i, count) == -1) return -1;
        return 0;
};

int get_tty_in_line (struct lb_tty * tty) {
        ssize_t nbytes;
        size_t len = tty->ins->maxsize;
        char * tbuf = calloc(len, sizeof(char));
        if (tbuf == NULL) {
                errno = ENOMEM;
                return -1;
        };
        size_t ti;
        bool esc = false;
        if (clear_tty_in(tty) == -1) return -1;
        struct lb_string * tmp;
        while (true) {
                nbytes = read(tty->ins->fileno, tbuf, len);
                if (nbytes < 0 && errno != EINTR) return -1;
                ti = 0;
                while (ti < nbytes) {
                        switch (tbuf[ti]) {
                                case AS_ESCAPE:
                                        if (esc) {
                                                tmp = init_ntstring("\\");
                                                if (tmp == NULL) {
                                                        errno = ENOMEM;
                                                        return -1;
                                                };
                                                if (put_to_tty_in(tty, tmp) == -1) return -1;
                                                free_string(&tmp);
                                                esc = false;
                                        } else {
                                                esc = true;
                                        };
                                        break;
                                case AS_RETURN: case AS_NEWLINE: return 0;
                                case AS_DELETE:
                                        take_from_tty_in(tty, -1);
                                break;
                                default:
                                        tmp = init_ntstring(&tbuf[ti]);
                                        if (tmp == NULL) {
                                                errno = ENOMEM;
                                                return -1;
                                        };
                                        if (put_to_tty_in(tty, tmp) == -1) return -1;
                                        free_string(&tmp);
                                break;
                        };
                };
        };
};
