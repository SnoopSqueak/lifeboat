#include "lb_tty.h"

#include <termios.h>
#include <unistd.h>

static int curi;
static int linei;
static int nrow;
static int ncol;
static int linelen;
static FILE * fin;
static FILE * fout;
static int fdin;
static int fdout;
static char * strin;
static char * strout;
static struct termios * tcattr;

int tty_init (int * numcol, int * numrow, int * maxlinelen) {
        curi = 0;
        linei = 0;
        nrow = *numrow;
        ncol = *numcol;
        linelen = *maxlinelen;
        fin = stdin;
        fdin = STDIN_FILENO;
        fout = stdout;
        fdout = STDOUT_FILENO;
        strin = calloc(*maxlinelen, sizeof(char));
        int outlen = (*maxlinelen) * (*numrow - 1) * (*numcol);
        strout = calloc(outlen, sizeof(char));
        int termsize = sizeof(struct termios);
        tcattr = malloc(termsize);
        struct termios * modtcattr = malloc(termsize);
        if (tcgetattr(fdout, modtcattr) != 0) return -1;
        memcpy(tcattr, modtcattr, termsize);
        modtcattr->c_lflag &= ~(ECHO | ECHONL | ICANON);
        if (tcsetattr(fdout, TCSADRAIN, modtcattr) != 0) return -1;
        free(modtcattr);
        return 0;
};

int tty_draw () {
        if (fputs(strout, fout) == EOF) return -1;
        char * moveseq = calloc(12, sizeof(char));
        int col = 1;

        char * tstr = malloc(32 * sizeof(char));
        tstr[0] = LBF_NULL;
        if (string_from_int(tstr, &curi) != 0) return -1;
        if (string_cat(tstr, " <- curi\n\n") != 0) return -1;
        if (fputs(tstr, fout) == EOF) return -1;
        free(tstr);

        if (string_move_cur(moveseq, &col, &nrow) != 0) return -1;
        int mslen;
        if (count_string_size(&mslen, moveseq) != 0) return -1;
        char * inslice = calloc(ncol + mslen, sizeof(char));
        if (string_cat(inslice, moveseq) != 0) return -1;
        mslen--;
        if (string_copy(inslice, &mslen, strin, &linei, &ncol) != 0) return -1;
        col = curi - linei + 1;
        if (string_move_cur(moveseq, &col, &nrow) != 0) return -1;
        if (string_cat(inslice, moveseq) != 0) return -1;
        if (fputs(inslice, fout) == EOF) return -1;
        free(moveseq);
        free(inslice);
        return 0;
};

int tty_put_out (char * source) {
        strout[0] = LBF_NULL;
        if (string_cat(strout, LBF_RESET) != 0) return -1;
        if (string_cat(strout, source) != 0) return -1;
        if (tty_draw() != 0) return -1;
        return 0;
};

int tty_get_in (char * dest) {
        int i = 0;
        strin[0] = LBF_NULL;
        curi = 0;
        linei = 0;
        char c;
        bool isesc = false;
        // big enough to hold any escape sequence
        int elen = 12;
        char * ebuf = calloc(elen, sizeof(char));
        int ei;
        int ilen;
        while (feof(fin) == 0) {
                c = fgetc(fin);
                if (c == EOF || ferror(fin) != 0) {
                        errno = EIO;
                        return -1;
                };
                if (c == LBF_RETURN || c == LBF_NEWLINE) {
                        break;
                };
                if (c == LBF_ESCAPE) {
                        isesc = true;
                        ebuf[0] = LBF_NULL;
                        ei = 0;
                };
                if (!isesc) {
                        if (i + 1 < linelen - 1) {
                                if (curi < i) {
                                        if (string_ins_char(strin, &curi, &c) != 0) return -1;
                                } else {
                                        strin[i] = c;
                                        strin[i+1] = LBF_NULL;
                                };
                                i++;
                                if (i >= ncol) {
                                        linei++;
                                } else {
                                        curi++;
                                };
                        };
                } else {
                        if (ei >= elen) {
                                errno = ERANGE;
                                free(ebuf);
                                return -1;
                        };
                        ebuf[ei] = c;
                        ebuf[ei+1] = LBF_NULL;
                        ei++;
                        if (string_is_equal(ebuf, LBF_LEFT)) {
                                if (curi > 0) {
                                        curi--;
                                        if (curi <= linei) {
                                                linei = curi;
                                        };
                                };
                                isesc = false;
                        } else if (string_is_equal(ebuf, LBF_RIGHT)) {
                                if (count_string_size(&ilen, strin) != 0) return -1;
                                if (curi < ilen - 1) {
                                        curi++;
                                        if (curi > linei + ncol) {
                                                linei = curi - ncol;
                                        };
                                };
                                isesc = false;
                        } else if (string_is_equal(ebuf, LBF_UP)) {
                                isesc = false;
                        } else if (string_is_equal(ebuf, LBF_DOWN)) {
                                isesc = false;
                        };
                };
                if (tty_draw() != 0) return -1;
        };
        free(ebuf);
        if (string_cat(dest, strin) != 0) return -1;
        return 0;
};

int tty_free () {
        if (string_cat(strout, "\n") != 0) return -1;
        if (string_cat(strin, "\n") != 0) return -1;
        if (tty_draw() != 0) return -1;
        if (tcsetattr(fdout, TCSADRAIN, tcattr) != 0) return -1;
        free(tcattr);
        free(strin);
        free(strout);
        tcattr = NULL;
        strin = NULL;
        strout = NULL;
        return 0;
};
