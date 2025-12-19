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
        
        
        //~ char tbuf[len];
        //~ ssize_t c = 0;
        //~ size_t ti = 0;
        //~ size_t dmax = 0;
        //~ size_t dd;
        //~ ssize_t ei = -1;
        //~ char ebuf[4];
        //~ clearin(tty);
        //~ while (term->ini < len) {
                //~ c = read(fileno(term->instr), tbuf, len);
                //~ if (c < 0 && errno == EINTR) continue;
                //~ ti = 0;
                //~ while (ti < c) {
                        //~ switch (tbuf[ti]) {
                                //~ case AS_ESCAPE:
                                        //~ if (ei == -1) {
                                                //~ ei = 0;
                                        //~ }
                                        //~ break;
                                //~ case AS_RETURN: case AS_NEWLINE: return 0;
                                //~ case AS_DELETE:
                                        //~ if (term->ini > 0) {
                                                //~ dd = term->ini - 1;
                                                //~ while (dd <= dmax) {
                                                        //~ dest[dd] = dest[dd+1];
                                                        //~ dd++;
                                                //~ }
                                                //~ term->ini--;
                                                //~ dmax--;
                                        //~ }
                                //~ break;
                        //~ }
                        //~ if (ei > -1) {
                                //~ ebuf[ei] = tbuf[ti];
                                //~ ei++;
                                //~ if (ei == 3 && ebuf[1] == AS_OSB) {
                                        //~ switch (ebuf[2]) {
                                                //~ case AS_LEFT:
                                                        //~ if (term->ini > 0) term->ini--;
                                                //~ break;
                                                //~ case AS_RIGHT:
                                                        //~ if (term->ini < dmax) term->ini++;
                                                //~ break;
                                                //~ case AS_UP:
                                                        //~ print_to_term(term, LB_DOWN, 4);
                                                //~ break;
                                        //~ }
                                        //~ ei = -1;
                                //~ }
                        //~ } else {
                                //~ if (term->ini < dmax) {
                                        //~ dd = dmax;
                                        //~ while (dd > term->ini) {
                                                //~ dest[dd] = dest[dd - 1];
                                                //~ dd--;
                                        //~ }
                                        
                                //~ }
                                //~ dest[term->ini] = tbuf[ti];
                                //~ dmax++;
                                //~ term->ini++;
                        //~ }
                        //~ ti++;
                        //~ if (ti >= len) {
                                //~ errno = ERANGE;
                                //~ return -1;
                        //~ }
                //~ }
        //~ };
        //~ return 0;
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
