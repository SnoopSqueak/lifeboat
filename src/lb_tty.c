#include "lb_tty.h"

struct lb_stream * init_stream (FILE * file, size_t*  maxsize) {
        struct lb_stream * stream = malloc(sizeof(struct lb_stream));
        stream->file = file;
        stream->string = init_ntstring("");
        stream->maxsize = *maxsize;
        return stream;
};

int free_stream (struct lb_stream * stream) {
        free_string(stream->string);
        free(stream);
        return 0;
};

struct lb_tty * init_tty (FILE * fin, size_t * lin, FILE * fout, size_t * lout, size_t * nrow, size_t * ncol) {
        size_t termsize = sizeof(struct termios);
        struct lb_tty * tty = malloc(sizeof(struct lb_tty));
        tty->ins = init_stream(fin, lin);
        tty->ins->maxsize = *lin;
        tty->outs = init_stream(fout, lout);
        tty->outs->maxsize = *lout;
        tty->curi = 0;
        tty->nrow = *nrow;
        tty->ncol = *ncol;
        tty->attr = malloc(termsize);
        tty->attrcpy = malloc(termsize);
        // Store old tty attributes, set new tty attributes.
        setvbuf(fout, NULL, _IONBF, BUFSIZ);
        tcgetattr(fileno(fout), tty->attr);
        memcpy(tty->attrcpy, tty->attr, termsize);
        tty->attr->c_lflag &= (ECHO | ISIG);
        tty->attr->c_lflag &= ~(ICANON);
        //~ tty->attr->c_lflag &= (ISIG);
        //~ tty->attr->c_lflag &= ~(ECHO | ICANON);
        tcsetattr(fileno(fout), TCSADRAIN, tty->attr);
        return tty;
};

int free_tty (struct lb_tty * tty) {
        tcsetattr(fileno(tty->outs->file), TCSADRAIN, tty->attrcpy);
        free_stream(tty->ins);
        free_stream(tty->outs);
        free(tty->attr);
        free(tty->attrcpy);
        free(tty);
        return 0;
};

int clear_tty_in (struct lb_tty * tty) {
        take_from_string(tty->ins->string, 0, -1);
        tty->curi = 0;
        return 0;
};

int clear_tty_out (struct lb_tty * tty) {
        struct lb_string * orig = init_ntstring(LB_ORIGIN);
        put_to_tty_out(tty, orig);
        free(orig);
        struct lb_string * clear = init_ntstring(LB_CLEAR);
        put_to_tty_out(tty, clear);
        free(clear);
        clear_string(tty->outs->string);
        return 0;
};

int put_to_tty_out (struct lb_tty * tty, struct lb_string * string) {
        put_in_string(tty->outs->string, -1, string);
        write(fileno(tty->outs->file), tty->outs->string->ntstring, tty->outs->string->ntsize);
        return 0;
};

int put_to_tty_in (struct lb_tty * tty, struct lb_string * string) {
        put_in_string(tty->ins->string, tty->curi, string);
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
        take_from_string(tty->ins->string, i, count);
        return 0;
};

int get_tty_in_line (struct lb_tty * tty) {
        ssize_t nbytes;
        size_t len = tty->ins->maxsize;
        char * tbuf = calloc(len, sizeof(char));
        size_t ti;
        //~ char * dest = tty->ins->buf;
        bool esc = false;
        clear_tty_in(tty);
        struct lb_string * tmp;
        while (true) {
                nbytes = read(fileno(tty->ins->file), tbuf, len);
                if (nbytes < 0 && errno != EINTR) return errno;
                ti = 0;
                while (ti < nbytes) {
                        switch (tbuf[ti]) {
                                case AS_ESCAPE:
                                        if (esc) {
                                                tmp = init_ntstring("\\");
                                                put_to_tty_in(tty, tmp);
                                                free(tmp);
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
                                        put_to_tty_in(tty, tmp);
                                        free(tmp);
                                break;
                        };
                };
        };
};
