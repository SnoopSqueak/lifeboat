#include "lb_term.h"

struct lb_term * make_term (FILE * outdest) {
        struct lb_term * new_term = malloc(sizeof(struct lb_term));
        size_t term_size = sizeof(struct termios);
        new_term->attr = malloc(term_size);
        new_term->attrcpy = malloc(term_size);
        new_term->outstr = outdest;
        setvbuf(outdest, NULL, _IONBF, BUFSIZ);
        tcgetattr(fileno(outdest), new_term->attr);
        memcpy(new_term->attrcpy, new_term->attr, term_size);
        //~ new_term->attr->c_lflag &= ~(ECHO | ECHONL | ICANON);
        new_term->attr->c_lflag &= ~(ICANON);
        tcsetattr(fileno(outdest), TCSADRAIN, new_term->attr);
        return new_term;
}

int free_term(struct lb_term * term) {
        tcsetattr(fileno(term->outstr), TCSADRAIN, term->attrcpy);
        free(term->attr);
        free(term->attrcpy);
        free(term);
        return 0;
}

int print_to_term(struct lb_term * term, char * msg, size_t len) {
        if (len == 0) return 0;
        return write(fileno(term->outstr), msg, len);
}
