#include "lb_tty.h"

int curi;
int linei;
int nrow;
int ncol;
int linelen;
struct lb_str *strin;
struct lb_str *strout;
struct lb_file *fin;
struct lb_file *fout;
struct lb_tty_cfg *cfg;

int tty_init (int *numcol, int *numrow, int *maxlinelen) {
        curi = 0;
        linei = 0;
        nrow = *numrow;
        ncol = *numcol;
        linelen = *maxlinelen;
        if (init_file_stdin(&fin) != 0) return -1;
        if (init_file_stdout(&fout) != 0) goto cleanfin;
        if (init_str_charcount(&strin, &linelen) != 0) goto cleanfiles;
        int outlen = (*numrow) * ((*numcol) + 1) * 2;
        if (init_str_charcount(&strout, &outlen) != 0) goto cleanstrin;
        if (init_tty_cfg(&cfg) != 0) goto cleanstrs;
        return 0;
cleanstrs:
        free_str(&strout);
cleanstrin:
        free_str(&strin);
cleanfiles:
        free_file(&fout);
cleanfin:
        free_file(&fin);
        return -1;
};

int tty_draw () {
        if (file_put_clear(fout) != 0) return -1;
        if (file_put_str(fout, strout) != 0) return -1;
        struct lb_str *inslice;
        int col = 1, islen = LBF_ESCSEQ_LEN;
        if (init_str_charcount(&inslice, &islen) != 0) return -1;
        if (fmt_move_cur(inslice, &col, &nrow) != 0) goto cleanins;
        if (str_cat_str(inslice, strin) != 0) goto cleanins;
        col = curi - linei + 1;
        if (fmt_move_cur(inslice, &col, &nrow) != 0) goto cleanins;
        if (file_put_str(fout, inslice) != 0) goto cleanins;
        free_str(&inslice);
        if (file_flush(fout) != 0) return -1;
        return 0;
cleanins:
        free_str(&inslice);
        return -1;
};

int tty_put_out (const struct lb_str *src) {
        if (fmt_reset(strout) != 0) return -1;
        if (str_cat_str(strout, src) != 0) return -1;
        if (tty_draw() != 0) return -1;
        return 0;
};

int tty_get_in (struct lb_str *dest) {
        char c;
        if (str_del_all(strin) != 0) return -1;
        while (LB_TRUE) {
                if (file_get_key(&c, fin) != 0) return -1;
                if (c == '\n') break;
                if (str_cat_char(strin, &c) != 0) return -1;
        };
        if (str_cat_str(dest, strin) != 0) return -1;
        return 0;
};

int tty_free () {
        int result = 0;
        // if (tty_draw() != 0) result = -1;
        if (free_tty_cfg(&cfg) != 0) result = -1;
        if (free_file(&fin) != 0) result = -1;
        if (free_file(&fout) != 0) result = -1;
        if (free_str(&strin) != 0) result = -1;
        if (free_str(&strout) != 0) result = -1;
        return result;
};
