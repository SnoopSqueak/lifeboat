#include "lb_view.h"

int view_landing(const int *ncol, const int *nrow, const char *swname,
                 const char *swvers) {
        int x = 1;
        int y = 1;
        struct lb_str *lines, *tmp;
        if (init_str_charcount(&lines, nrow) != 0) return -1;
        if (init_str_chars(&tmp, swname) != 0) return -1;
        if (fmt_align_left(lines, tmp, &x, &y) != 0) goto cleantmp;
        x = 39;
        if (str_del_all(tmp) != 0) goto cleantmp;
        if (str_cat_chars(tmp, "status: offline") != 0) goto cleantmp;
        if (fmt_align_center(lines, tmp, &x, &y) != 0) goto cleantmp;
        x = 80;
        if (str_del_all(tmp) != 0) goto cleantmp;
        if (str_cat_chars(tmp, swvers) != 0) goto cleantmp;
        if (fmt_align_right(lines, tmp, &x, &y) != 0) goto cleantmp;
        free_str(&tmp);
        if (tty_put_out(lines) != 0) goto cleanlines;
        free_str(&lines);
        return 0;
cleantmp:
        free_str(&tmp);
cleanlines:
        free_str(&lines);
        return -1;
};
