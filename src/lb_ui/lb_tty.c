#include "lb_tty.h"

static int curi;
static int linei;
static int nrow;
static int ncol;
static int linelen;
static char *strin;
static char *strout;
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
        strin = calloc(*maxlinelen, sizeof(char));
        if (strin == NULL) goto cleanfiles;
        int outlen = (*maxlinelen) * (*numrow - 1) * (*numcol);
        strout = calloc(outlen, sizeof(char));
        if (strout == NULL) goto cleanstrin;
        if (tty_cfg_init(&cfg) != 0) goto cleanstrs;
        return 0;
cleanstrs:
        free(strout);
cleanstrin:
        free(strin);
cleanfiles:
        file_free(&fout);
cleanfin:
        file_free(&fin);
        return -1;
};

int tty_draw () {
        printf("Putting string...\n");
        if (file_put_string(fout, strout) != 0) return -1;
        printf("Put string.\n");
        char *moveseq = calloc(12, sizeof(char));
        if (moveseq == NULL) return -1;
        int col = 1;
        if (string_move_cur(moveseq, &col, &nrow) != 0) goto cleanms;
        int mslen;
        if (string_count_size(&mslen, moveseq) != 0) goto cleanms;
        char *inslice = calloc(ncol + mslen, sizeof(char));
        if (inslice == NULL) goto cleanms;
        if (string_cat(inslice, moveseq) != 0) goto cleanins;
        mslen--;
        if (string_copy(inslice, &mslen, strin, &linei, &ncol) != 0) goto cleanins;
        col = curi - linei + 1;
        if (string_move_cur(moveseq, &col, &nrow) != 0) goto cleanins;
        if (string_cat(inslice, moveseq) != 0) goto cleanins;
        if (file_put_string(fout, inslice) != 0) goto cleanins;
        free(inslice);
        free(moveseq);
        return 0;
cleanins:
        free(inslice);
cleanms:
        free(moveseq);
        return -1;
};

int tty_put_out (char *source) {
        strout[0] = LBF_NULL;
        if (string_cat(strout, LBF_RESET) != 0) return -1;
        if (string_cat(strout, source) != 0) return -1;
        if (tty_draw() != 0) return -1;
        return 0;
};

int tty_get_in (char * dest) {
        struct lb_kbevent * keybuf = malloc(sizeof(keybuf));
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
        if (string_cat(strout, "\n") != 0) result = -1;
        if (string_cat(strin, "\n") != 0) result = -1;
        if (tty_draw() != 0) result = -1;
        if (tty_cfg_free(&cfg) != 0) result = -1;
        if (file_free(&fin) != 0) result = -1;
        if (file_free(&fout) != 0) result = -1;
        free(strin);
        free(strout);
        strin = NULL;
        strout = NULL;
        return result;
};
