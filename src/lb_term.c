#include "lb_term.h"

#include <errno.h>

struct lb_term * get_term (FILE * insrc, size_t inmax, FILE * outdest, size_t outmax) {
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
        setvbuf(outdest, NULL, _IONBF, BUFSIZ);
        tcgetattr(fileno(outdest), new_term->attr);
        memcpy(new_term->attrcpy, new_term->attr, term_size);
        //~ new_term->attr->c_iflag &= IUTF8;
        //~ new_term->attr->c_lflag &= ~(ECHO | ECHONL | ICANON);
        new_term->attr->c_lflag &= (ECHO | ISIG);
        new_term->attr->c_lflag &= ~(ICANON);
        tcsetattr(fileno(outdest), TCSADRAIN, new_term->attr);
        return new_term;
}

int get_input (struct lb_term * term, char * dest, size_t len) {
        char tbuf[len];
        ssize_t c = 0;
        size_t ti = 0;
        size_t di = 0;
        size_t dmax = 0;
        size_t dd;
        while (dest[di] != '\0') {
                dmax++;
                di++;
        }
        di = 0;
        // todo: overflow error if too much?
        while (di < len) {
                c = read(fileno(term->instr), tbuf, len);
                if (c < 0 && errno == EINTR) continue;
                ti = 0;
                while (ti < c) {
                        switch (tbuf[ti]) {
                                case AS_RETURN: case AS_NEWLINE: return 0;
                                // todo: dis don't work
                                case AS_DELETE:
                                        if (di > 0) {
                                                dd = di - 1;
                                                while (dd <= dmax) {
                                                        dest[dd] = dest[dd+1];
                                                        dd++;
                                                }
                                                di--;
                                                dmax--;
                                        }
                                break;
                        }
                        dest[di] = tbuf[ti];
                        dmax++;
                        di++;
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

int print_to_term(struct lb_term * term, char * msg, size_t len) {
        if (len == 0) return 0;
        return write(fileno(term->outstr), msg, len);
}
