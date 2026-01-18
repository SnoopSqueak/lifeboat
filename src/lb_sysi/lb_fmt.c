#include "lb_fmt.h"

const struct lb_str LBF_CLEAR = {.chars = "\033[2J", .size = 5};
const struct lb_str LBF_RESET = {.chars = "\033[2J\033[1;1H", .size = 11};
const struct lb_str LBF_NEWLINE = {.chars = "\012", .size = 2};
// const struct lb_str LBF_NEWLINE = {.chars = "N", .size = 2};
// const struct lb_str LBF_FG_RESET = {.chars = "\033[0m", .size = 5};
const struct lb_str LBF_ESCAPE = {.chars = "\033", .size = 2};
const struct lb_str LBF_OSB = {.chars = "[", .size = 2};
const struct lb_str LBF_SEMICOLON = {.chars = ";", .size = 2};
const struct lb_str LBF_ES_MOVE = {.chars = "H", .size = 2};
const struct lb_str LBF_ES_GRAPHICS = {.chars = "m", .size = 2};

// #define LBF_ESCAPE '\033'
// #define LBF_OSB '['
// #define LBF_SEMICOLON ';'

// // Escape Sequence end codes
// #define LBF_ES_UP 'A'
// #define LBF_ES_DOWN 'B'
// #define LBF_ES_LEFT 'C'
// #define LBF_ES_RIGHT 'D'
// #define LBF_ES_MOVE 'H'
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

// Foreground colors
// #define LBF_FG_BLACK "\033[30m"
// #define LBF_FG_RED "\033[31m"
// #define LBF_FG_GREEN "\033[32m"
// #define LBF_FG_YELLOW "\033[33m"
// #define LBF_FG_BLUE "\033[34m"
// #define LBF_FG_MAGENTA "\033[35m"
// #define LBF_FG_CYAN "\033[36m"
// #define LBF_FG_WHITE "\033[37m"
// #define LBF_FG_BR_BLACK "\033[90m"
// #define LBF_FG_BR_RED "\033[91m"
// #define LBF_FG_BR_GREEN "\033[92m"
// #define LBF_FG_BR_YELLOW "\033[93m"
// #define LBF_FG_BR_BLUE "\033[94m"
// #define LBF_FG_BR_MAGENTA "\033[95m"
// #define LBF_FG_BR_CYAN "\033[96m"
// #define LBF_FG_BR_WHITE "\033[97m"
// #define LBF_FG_RESET "\033[0m"

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

int fmt_fg_color (struct lb_str *dest, const int *lbcolor, const int *isbr) {
        const int colbase = 30;
        int colcode = colbase + *lbcolor;
        if (*isbr) colcode += 60;
        if (fmt_es_pre(dest) != 0) return -1;
        if (fmt_int(dest, &colcode) != 0) return -1;
        if (str_cat_str(dest, &LBF_ES_GRAPHICS) != 0) return -1;
        return 0;
};

int fmt_align_left(struct lb_str *dest, const struct lb_str *src, const int *x, const int *y) {
        int ssize, newx = *x, delamt, i = 0;
        struct lb_str *scpy;
        if (init_str_str(&scpy, src) != 0) return -1;
        if (count_str_vischars(&ssize, scpy) != 0) goto cleanscpy;
        if (newx < 1) {
                delamt = (-newx) + 1;
                if (str_del_charcount(scpy, &i, &delamt) != 0) goto cleanscpy;
        };
        if (fmt_move_cur(dest, x, y) != 0) goto cleanscpy;
        if (str_cat_str(dest, scpy) != 0) goto cleanscpy;
        free_str(&scpy);
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
        *dest = 1;
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
                if (i >= LBF_MAX_STR_LEN - 1) {
                        errno = ERANGE;
                        return -1;
                };
                i++;
        };
        return 0;
};
