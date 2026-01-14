#include "lb_tty.h"

static int curi;
static int linei;
static int nrow;
static int ncol;
static int linelen;
static struct lb_str *strin;
static struct lb_str *strout;
static struct lb_file *fin;
static struct lb_file *fout;
static struct lb_tty_cfg *cfg;

int tty_init (int *numcol, int *numrow, int *maxlinelen) {
        curi = 0;
        linei = 0;
        nrow = *numrow;
        ncol = *numcol;
        linelen = *maxlinelen;
        if (file_init_tty_in(&fin) != 0) return -1;
        if (file_init_tty_out(&fout) != 0) goto cleanfin;
        if (str_from_charcount(&strin, maxlinelen) != 0) goto cleanfiles;
        int outlen = (*maxlinelen) * (*numrow - 1) * (*numcol);
        if (str_from_charcount(&strout, &outlen) != 0) goto cleanstrin;
        if (tty_cfg_init(&cfg) != 0) goto cleanstrs;
        return 0;
cleanstrs:
        free_str(&strout);
cleanstrin:
        free_str(&strin);
cleanfiles:
        file_free(&fout);
cleanfin:
        file_free(&fin);
        return -1;
};

int tty_draw () {
        if (file_put_str(fout, strout) != 0) return -1;
        struct lb_str *inslice;
        int col = 1, islen = LBF_ESCSEQ_LEN;
        if (str_from_charcount(&inslice, &islen) != 0) return -1;
        if (fmt_move_cur(inslice, &col, &nrow) != 0) goto cleanins;
        if (str_cat_str(inslice, strin) != 0) goto cleanins;
        col = curi - linei + 1;
        if (fmt_move_cur(inslice, &col, &nrow) != 0) goto cleanins;
        if (file_put_str(fout, inslice) != 0) goto cleanins;
        free_str(&inslice);
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

int tty_get_in (char *dest) {
        struct lb_kbevent *keybuf = malloc(sizeof(struct lb_kbevent));
        if (file_get_key(keybuf, fin) != 0) goto cleankeybuf;
        printf("\nRead: '%3d'.\n", keybuf->code);
        free(keybuf);
        return 0;
cleankeybuf:
        free(keybuf);
        return -1;
};

int tty_free () {
        int result = 0;
        if (fmt_newline(strout) != 0) return -1;
        if (fmt_newline(strin) != 0) return -1;
        if (tty_draw() != 0) result = -1;
        if (tty_cfg_free(&cfg) != 0) result = -1;
        if (file_free(&fin) != 0) result = -1;
        if (file_free(&fout) != 0) result = -1;
        free_str(&strin);
        free_str(&strout);
        return result;
};
