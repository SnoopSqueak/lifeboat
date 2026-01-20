#include "lb_view.h"

int view_landing(const int *ncol, const int *nrow, const struct lb_str *swname,
                 const struct lb_str *swvers) {
        int x = 1;
        int y = 1;
        struct lb_str *lines, *tmp;
        if (init_str_charcount(&lines, nrow) != 0) return -1;
        if (init_str_charcount(&tmp, nrow) != 0) return -1;
        if (fmt_align_left(lines, swname, &x, &y) != 0) goto cleantmp;
        x = 39;
        if (str_cat_chars(tmp, "status: offline") != 0) goto cleantmp;
        if (fmt_align_left(lines, tmp, &x, &y) != 0) goto cleantmp;
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
