#include "lb_tty.h"

struct lb_stream * init_stream (FILE * file, int * maxsize) {
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

int free_stream (struct lb_stream ** stream) {
        if (free_string(&((*stream)->string)) == -1) return -1;
        free(*stream);
        *stream = NULL;
        return 0;
};

struct lb_tty * init_tty (FILE * fin, int * lin, FILE * fout, int * lout, int * nrow, int * ncol) {
        struct lb_tty * tty = malloc(sizeof(struct lb_tty));
        tty->ins = init_stream(fin, lin);
        tty->outs = init_stream(fout, lout);
        tty->ins->maxsize = *lin;
        tty->outs->maxsize = *lout;
        tty->curi = 0;
        tty->nrow = *nrow;
        tty->ncol = *ncol;
        return tty;
};

int free_tty (struct lb_tty ** tty) {
        bool tripped = false;
        if (free_stream(&((*tty)->ins)) == -1) tripped = true;
        if (free_stream(&((*tty)->outs)) == -1) tripped = true;
        free(*tty);
        *tty = NULL;
        return tripped ? -1 : 0;
};

int put_to_tty_out (struct lb_tty * tty, struct lb_string * string) {
        if (put_in_string(tty->outs->string, -1, string) == -1) return -1;
        fputs(string->ntstring, tty->outs->file);
        return 0;
};

int put_to_tty_in (struct lb_tty * tty, struct lb_string * string) {
        if (put_in_string(tty->ins->string, tty->curi, string) == -1) return -1;
        tty->curi += string->ntsize - 1;
        return 0;
};

int take_from_tty_in (struct lb_tty * tty, int count) {
        if (count == 0) return 0;
        if (count < 0) count = tty->curi;
        if (tty->curi < count) {
                errno = ERANGE;
                return -1;
        };
        tty->curi -= count;
        if (take_from_string(tty->ins->string, tty->curi, count) == -1) return -1;
        return 0;
};

int get_tty_in_line (struct lb_tty * tty) {
        //~ int nbytes;
        int len = tty->ins->maxsize;
        char * tbuf = calloc(len, sizeof(char));
        if (tbuf == NULL) {
                errno = ENOMEM;
                return -1;
        };
        //~ int ti;
        //~ bool esc = false;
        if (clear_tty_in(tty) == -1) return -1;
        struct lb_string * tmp;
        tmp = init_ntstring("Work in progress.");
        while (true) {
                if (fgets(tbuf, len, tty->ins->file) == NULL) return -1;
                put_to_tty_in(tty, tmp);
                //~ ti = 0;
                //~ while (tbuf[ti] != '\0') {
                        //~ switch (tbuf[ti]) {
                                //~ case AS_ESCAPE:
                                        //~ if (esc) {
                                                //~ tmp = init_ntstring("\\");
                                                //~ if (tmp == NULL) {
                                                        //~ errno = ENOMEM;
                                                        //~ return -1;
                                                //~ };
                                                //~ if (put_to_tty_in(tty, tmp) == -1) return -1;
                                                //~ free_string(&tmp);
                                                //~ esc = false;
                                        //~ } else {
                                                //~ esc = true;
                                        //~ };
                                        //~ break;
                                //~ case AS_RETURN: case AS_NEWLINE: return 0;
                                //~ case AS_DELETE:
                                        //~ take_from_tty_in(tty, -1);
                                //~ break;
                                //~ default:
                                        //~ tmp = init_ntstring(&tbuf[ti]);
                                        //~ if (tmp == NULL) {
                                                //~ errno = ENOMEM;
                                                //~ return -1;
                                        //~ };
                                        //~ if (put_to_tty_in(tty, tmp) == -1) return -1;
                                        //~ free_string(&tmp);
                                //~ break;
                        //~ };
                //~ };
        };
        return 0;
};

int clear_tty_in (struct lb_tty * tty) {
        //struct lb_string * clear = init_ntstring(home_and_clear());
        struct lb_string * clear = init_ntstring("\033[2J\033[1;1H");
        if (put_to_tty_in(tty, clear) == -1) return -1;
        if (free_string(&clear) == -1) return -1;
        if (clear_string(tty->ins->string) == -1) return -1;
        tty->curi = 0;
        return 0;
};

int clear_tty_out (struct lb_tty * tty) {
        // struct lb_string * clear = init_ntstring(home_and_clear());
        struct lb_string * clear = init_ntstring("\033[2J\033[1;1H");
        if (put_to_tty_out(tty, clear) == -1) return -1;
        if (free_string(&clear) == -1) return -1;
        if (clear_string(tty->outs->string) == -1) return -1;
        return 0;
};
