#ifndef LB_FMT_H
#define LB_FMT_H

#include "lb_bool.h"
#include "lb_str.h"

int fmt_clear (struct lb_str *dest);
int fmt_reset (struct lb_str *dest);
int fmt_newline (struct lb_str *dest);
int fmt_move_cur (struct lb_str *dest, const int *x, const int *y);
int fmt_align_left(struct lb_str *dest, const struct lb_str *src, const int *x, const int *y);
int fmt_align_center(struct lb_str *dest, const struct lb_str *src, const int *x, const int *y);
int fmt_align_right(struct lb_str *dest, const struct lb_str *src, const int *x, const int *y);
int count_str_vischars (int *dest, const struct lb_str *src);
int str_del_vischars (struct lb_str *dest, const int *i, const int *count);

#endif /* LB_FMT_H */
