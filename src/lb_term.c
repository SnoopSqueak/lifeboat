#include "lb_term.h"

struct lb_term * get_term (FILE * insrc, size_t inmax, FILE * outdest, size_t outmax, size_t rowcount, size_t colcount) {
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
        new_term->colcount = colcount;
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
                        if (ti >= len) {
                                errno = ERANGE;
                                return -1;
                        }
                }
        };
        return 0;
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
        return print_to_term(term, LB_CLEAR, 3);
}

int print_to_term(struct lb_term * term, char * msg, size_t len) {
        if (len == 0) return 0;
        return write(fileno(term->outstr), msg, len);
}

int print_lines(struct lb_term * term, char * lines) {
        write(fileno(term->outstr), lines, term->colcount * term->rowcount);
        return 0;
}
