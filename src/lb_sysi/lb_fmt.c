#include "lb_fmt.h"

const struct lb_str LBF_CLEAR = {.chars = "\033[2J", .size = 5};
const struct lb_str LBF_RESET = {.chars = "\033[2J\033[1;1H", .size = 11};
const struct lb_str LBF_NEWLINE = {.chars = "\012", .size = 2};
const struct lb_str LBF_ESCAPE = {.chars = "\033", .size = 2};
const struct lb_str LBF_OSB = {.chars = "[", .size = 2};
const struct lb_str LBF_SEMICOLON = {.chars = ";", .size = 2};
const struct lb_str LBF_ES_MOVE = {.chars = "H", .size = 2};
const struct lb_str LBF_ES_GRAPHICS = {.chars = "m", .size = 2};

// // Escape Sequence end codes
// #define LBF_ES_UP 'A'
// #define LBF_ES_DOWN 'B'
// #define LBF_ES_LEFT 'C'
// #define LBF_ES_RIGHT 'D'
// #define LBF_ES_CLEAR 'J'
// #define LBF_ES_GRAPHICS 'm'
// #define LBF_ES_ALIGN_LEFT 'L'
// #define LBF_ES_ALIGN_CENTER 'C'
// #define LBF_ES_ALIGN_RIGHT 'R'
//
// // Custom escape sequences
// #define LBF_ALIGN_LEFT "\033[[*;L"
// #define LBF_ALIGN_CENTER "\033[[*;C"
// #define LBF_ALIGN_RIGHT "\033[[*;R"
//
// // ANSI escape sequences
// #define LBF_CLEAR "\033[2J"
// #define LBF_RESET "\033[2J\033[1;1H"
// #define LBF_UP "\033[A"
// #define LBF_DOWN "\033[B"
// #define LBF_RIGHT "\033[C"
// #define LBF_LEFT "\033[D"

// #define LBF_DELETE '\177'

int is_seq_end (int *dest, const char *src) {
        switch (*src) {
                case 'H': case 'J': case 'm':
                        *dest = LB_TRUE;
                        break;
                default:
                        *dest = LB_FALSE;
                        break;
        };
        return 0;
};

int fmt_int (struct lb_str *dest, const int *src) {
        struct lb_str *istr;
        if (init_str_int(&istr, src) != 0) return -1;
        if (str_cat_str(dest, istr) != 0) goto cleanstr;
        if (free_str(&istr) != 0) return -1;
        return 0;
cleanstr:
        free_str(&istr);
        return -1;
};

int fmt_clear (struct lb_str *dest) {
        if (str_cat_str(dest, &LBF_CLEAR) != 0) return -1;
        return 0;
};

int fmt_reset (struct lb_str *dest) {
        if (str_cat_str(dest, &LBF_RESET) != 0) return -1;
        return 0;
};

int fmt_newline (struct lb_str *dest) {
        if (str_cat_str(dest, &LBF_NEWLINE) != 0) return -1;
        return 0;
};

int fmt_es_pre (struct lb_str *dest) {
        if (str_cat_str(dest, &LBF_ESCAPE) != 0) return -1;
        if (str_cat_str(dest, &LBF_OSB) != 0) return -1;
        return 0;
};

int fmt_move_cur (struct lb_str *dest, const int *x, const int *y) {
        if (fmt_es_pre(dest) != 0) return -1;
        if (fmt_int(dest, y) != 0) return -1;
        if (str_cat_str(dest, &LBF_SEMICOLON) != 0) return -1;
        if (fmt_int(dest, x) != 0) return -1;
        if (str_cat_str(dest, &LBF_ES_MOVE) != 0) return -1;
        return 0;
};

int fmt_align_left(struct lb_str *dest, const struct lb_str *src, const int *x, const int *y) {
        int newx = *x, i = 0, ssize;
        struct lb_str *scpy;
        if (init_str_str(&scpy, src) != 0) return -1;
        if (*x < 1) {
                i = 1 - (*x);
                if (str_del_charcount(scpy, &i, &i) != 0) goto cleanscpy;
        };
        if (count_str_vischars(&ssize, scpy) != 0) goto cleanscpy;
        // TODO: cut off text that goes offscreen? pass in ncol?
        if (fmt_move_cur(dest, &newx, y) != 0) return -1;
        if (str_cat_str(dest, src) != 0) return -1;
        return 0;
cleanscpy:
        free_str(&scpy);
        return -1;
};

int fmt_align_center(struct lb_str *dest, const struct lb_str *src, const int *x, const int *y) {
        int ssize;
        if (count_str_vischars(&ssize, src) != 0) return -1;
        const int newx = *x - (ssize/2);
        return fmt_align_left(dest, src, &newx, y);
};

int fmt_align_right(struct lb_str *dest, const struct lb_str *src, const int *x, const int *y) {
        int ssize;
        if (count_str_vischars(&ssize, src) != 0) return -1;
        const int newx = *x - ssize;
        return fmt_align_left(dest, src, &newx, y);
};

int count_str_vischars (int *dest, const struct lb_str *src) {
        if (src == NULL || dest == NULL) return -1;
        int i = 0, isesc = LB_FALSE, isend;
        *dest = 0;
        while (src->chars[i] != '\0') {
                if (isesc) {
                        if (is_seq_end(&isend, &(src->chars[i])) != 0) return -1;
                        if (isend == LB_TRUE) isesc = LB_FALSE;
                } else {
                        if (src->chars[i] == LBF_ESCAPE.chars[0]) {
                                isesc = LB_TRUE;
                        } else if (src->chars[i] != '\n') {
                                (*dest)++;
                        };
                };
                i++;
                if (i >= LBF_MAX_STR_LEN - 1) {
                        errno = ERANGE;
                        return -1;
                };
        };
        return 0;
};
