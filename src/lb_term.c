#include "lb_term.h"

struct lb_term * get_term (FILE * insrc, size_t inmax, FILE * outdest, size_t outmax, size_t rowcount) {
        struct lb_term * new_term = malloc(sizeof(struct lb_term));
        size_t term_size = sizeof(struct termios);
        new_term->attr = malloc(term_size);
        new_term->attrcpy = malloc(term_size);
        new_term->instr = insrc;
        new_term->MAX_INBUF = inmax;
        new_term->outstr = outdest;
        new_term->MAX_OUTBUF = outmax;
        new_term->inbuf = malloc(inmax * sizeof(char));
        new_term->outbuf = malloc(outmax * sizeof(char));
        new_term->ini = 0;
        new_term->rowcount = rowcount;
        setvbuf(outdest, NULL, _IONBF, BUFSIZ);
        tcgetattr(fileno(outdest), new_term->attr);
        memcpy(new_term->attrcpy, new_term->attr, term_size);
        new_term->attr->c_lflag &= (ECHO | ISIG);
        new_term->attr->c_lflag &= ~(ICANON);
        tcsetattr(fileno(outdest), TCSADRAIN, new_term->attr);
        return new_term;
}

int clearinbuf (struct lb_term * term) {
        size_t i = 0;
        while (i < term->MAX_INBUF) term->inbuf[i++] = '\0';
        term->ini = 0;
        return 0;
}

int get_input (struct lb_term * term, char * dest, size_t len) {
        char tbuf[len];
        ssize_t c = 0;
        size_t ti = 0;
        size_t dmax = 0;
        size_t dd;
        ssize_t ei = -1;
        char ebuf[4];
        clearinbuf(term);
        // todo: overflow error if too much?
        while (term->ini < len) {
                c = read(fileno(term->instr), tbuf, len);
                if (c < 0 && errno == EINTR) continue;
                ti = 0;
                while (ti < c) {
                        switch (tbuf[ti]) {
                                case AS_ESCAPE:
                                        if (ei == -1) {
                                                ei = 0;
                                        }
                                        break;
                                case AS_RETURN: case AS_NEWLINE: return 0;
                                case AS_DELETE:
                                        if (term->ini > 0) {
                                                dd = term->ini - 1;
                                                while (dd <= dmax) {
                                                        dest[dd] = dest[dd+1];
                                                        dd++;
                                                }
                                                term->ini--;
                                                dmax--;
                                        }
                                break;
                        }
                        if (ei > -1) {
                                ebuf[ei] = tbuf[ti];
                                ei++;
                                if (ei == 3 && ebuf[1] == AS_OSB) {
                                        switch (ebuf[2]) {
                                                case AS_LEFT:
                                                        if (term->ini > 0) term->ini--;
                                                break;
                                                case AS_RIGHT:
                                                        if (term->ini < dmax) term->ini++;
                                                break;
                                                case AS_UP:
                                                        print_to_term(term, LB_DOWN, 4);
                                                break;
                                        }
                                        ei = -1;
                                }
                        } else {
                                if (term->ini < dmax) {
                                        dd = dmax;
                                        while (dd > term->ini) {
                                                dest[dd] = dest[dd - 1];
                                                dd--;
                                        }
                                        
                                }
                                dest[term->ini] = tbuf[ti];
                                dmax++;
                                term->ini++;
                        }
                        ti++;
                }
        };
        return -1;
}

int free_term(struct lb_term * term) {
        tcsetattr(fileno(term->outstr), TCSADRAIN, term->attrcpy);
        free(term->inbuf);
        free(term->outbuf);
        free(term->attr);
        free(term->attrcpy);
        free(term);
        return 0;
}

int clearterm (struct lb_term * term) {
        movcurtopleft(term);
        return print_to_term(term, LB_CLEAR, strlen(LB_CLEAR));
}

int movcurleft (struct lb_term * term) {
        movcurtopleft(term);
        size_t i = 0;
        while (i++ < term->rowcount) print_to_term(term, LB_NEWLINE, 4);
        return 0;
}

int movcurtopleft (struct lb_term * term) {
        return print_to_term(term, "\033[1;1H", 6);
}

int movcurbotleft (struct lb_term * term) {
        movcurtopleft(term);
        return printmany(term, LB_DOWN, 4, term->rowcount-1);
}

int printmany (struct lb_term * term, char * msg, size_t len, size_t amt) {
        while (amt-- > 0) print_to_term(term, msg, len);
        return 0;
}

int relmovcur (struct lb_term * term, int x, int y) {
        if (x > 0) printmany(term, LB_RIGHT, 4, x);
        else printmany(term, LB_LEFT, 4, abs(x));
        if (y > 0) printmany(term, LB_DOWN, 4, y);
        else printmany(term, LB_UP, 4, abs(y));
        return 0;
}

int print_to_term(struct lb_term * term, char * msg, size_t len) {
        if (len == 0) return 0;
        return write(fileno(term->outstr), msg, len);
}
