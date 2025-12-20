#include "lb_tty.h"

struct lb_strm * getstrm (FILE * file, size_t * len) {
        struct lb_strm * strm = malloc(sizeof(lb_strm));
        strm->file = file;
        strm->buf = calloc(*len, sizeof(char));
        strm->len = len;
        return strm;
};

int freestrm (struct lb_strm * strm) {
        free(strm->buf);
        free(strm);
        return 0;
};

//~ int strmins (struct lb_strm * strm, size_t index, char * msg) {
        //~ char c = msg[0];
        //~ size_t mi = 0;
        //~ while (c != '\0') {
                //~ if (index >= strm->len) {
                        //~ errno = ERANGE;
                        //~ return -1;  
                //~ };
                //~ strm->buf[index] = msg[mi];
                //~ index++;
                //~ mi++;
        //~ };
        //~ return 0;
//~ };

//~ int strmdel (struct lb_strm * strm, size_t index) {
        //~ while (index < strm->len) {
                //~ if (index >= strm->len) {
                        //~ errno = ERANGE;
                        //~ return -1;
                //~ }
                //~ strm->buf[index] = strm->buf[index++];
        //~ }
        //~ return 0;
//~ }

struct lb_tty * gettty (FILE * fin, size_t * lin, FILE * fout, size_t * lout, size_t * nrow, size_t * ncol) {
        size_t ttysize = sizeof(struct termios);
        struct lb_tty * tty = malloc(sizeof(struct lb_tty));
        tty->ins = getstrm(fin, len);
        tty->outs = getstrm(fout, lout);
        tty->curi = &0;
        tty->nrow = nrow;
        tty->ncol = ncol;
        tty->attr = malloc(ttysize);
        tty->attrcpy = malloc(ttysize);
        // Store old tty attributes, set new tty attributes.
        setvbuf(fout, NULL, _IONBF, BUFSIZ);
        tcgetattr(fileno(fout), tty->attr);
        memcpy(tty->attrcpy, tty->attr, ttysize);
        //~ tty->attr->c_lflag &= (ECHO | ISIG);
        //~ tty->attr->c_lflag &= ~(ICANON);
        tty->attr->c_lflag &= (ISIG);
        tty->attr->c_lflag &= ~(ECHO | ICANON);
        tcsetattr(fileno(fout), TCSADRAIN, tty->attr);
        return tty;
};

int freetty (struct lb_tty * tty) {
        tcsetattr(fileno(tty->outs->file), TCSADRAIN, tty->attrcpy);
        freestrm(tty->ins);
        freestrm(tty->outs);
        free(tty->attr);
        free(tty->attrcpy);
        free(tty);
        return 0;
};

int clearin (struct lb_tty * tty) {
        size_t i = 0;
        while (i < *(tty->ins->len)) tty->ins->buf[i++] = '\0';
        tty->curi = &0;
        return 0;
}

int getin (struct lb_tty * tty) {
        ssize_t nbytes;
        size_t len = *(tty->ins->len);
        char * tbuf = calloc(*(tty->ins->len), sizeof(char));
        size_t ti;
        char * dest = tty->ins->buf;
        bool esc = false;
        clearin(tty);
        while (true) {
                nbytes = read(fileno(tty->ins->file), tbuf, len);
                if (nbytes < 0 && errno != EINTR) return errno;
                ti = 0;
                while (ti < nbytes) {
                        switch (tbuf[ti]) {
                                case AS_ESCAPE:
                                        if (esc) {
                                                strmins(tty->ins, *(tty->curi), '\\');
                                                esc = false;
                                        } else {
                                                esc = true;
                                        }
                                        break;
                                case AS_RETURN: case AS_NEWLINE: return 0;
                                case AS_DELETE:
                                        if (*(tty->curi) > 0) {
                                                *(tty->curi)--;
                                                strmdel(tty->ins, tty->curi);
                                        };
                                break;
                                default:
                                        
                                break;
                        };
                };
        };
}

int clearterm (struct lb_tty * term) {
        print_to_term(term, LB_ORIGIN, 6);
        return print_to_term(term, LB_CLEAR, 3);
}

int print_to_term(struct lb_tty * term, char * msg, size_t len) {
        if (len == 0) return 0;
        return write(fileno(term->outstr), msg, len);
}

//~ int print_lines(struct lb_tty * term, char * lines) {
        //~ write(fileno(term->outstr), lines, term->colcount * term->rowcount);
        //~ return 0;
//~ }
