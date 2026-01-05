#include "lb_tty.h"

static int curi;
static int linesize;
static FILE * fin;
static FILE * fout;
static char * strin;
static char * strout;

int init_tty (int * numcol, int * numrow, int * maxlinelen) {
        linesize = *maxlinelen;
        curi = 0;
        fin = stdin;
        fout = stdout;
        strin = calloc(*maxlinelen, sizeof(char));
        int outlen = (*maxlinelen) * (*numrow - 1) * (*numcol);
        strout = calloc(outlen, sizeof(char));
        // TODO: set canonical, echo
        return 0;
};

int draw_tty () {
        if (fputs(strout, fout) == EOF) return -1;
        return 0;
};

int put_tty_out (char * source) {
        strout[0] = '\0';
        if (string_cat(strout, source) != 0) return -1;
        if (draw_tty() != 0) return -1;
        return 0;
};

int get_tty_in (char * dest) {
        dest[0] = 'H';
        dest[1] = 'i';
        dest[2] = '.';
        dest[3] = '\0';
        return 0;
};

int release_tty () {
        // restore terminal attributes
        return 0;
};
