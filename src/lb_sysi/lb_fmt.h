#ifndef LB_FMT_H
#define LB_FMT_H

#include "lb_bool.h"
#include "lb_str.h"

#define LB_COLOR_RESET -30
#define LB_COLOR_BLACK 0
#define LB_COLOR_RED 1
#define LB_COLOR_GREEN 2
#define LB_COLOR_YELLOW 3
#define LB_COLOR_BLUE 4
#define LB_COLOR_MAGENTA 5
#define LB_COLOR_CYAN 6
#define LB_COLOR_WHITE 7

int fmt_clear (struct lb_str *dest);
int fmt_reset (struct lb_str *dest);
int fmt_newline (struct lb_str *dest);
int fmt_move_cur (struct lb_str *dest, const int *x, const int *y);
int fmt_fg_color (struct lb_str *dest, const int *lbcolor, const int *isbr);
int fmt_align_left(struct lb_str *dest, const struct lb_str *src, const int *x, const int *y);
int fmt_align_center(struct lb_str *dest, const struct lb_str *src, const int *x, const int *y);
int fmt_align_right(struct lb_str *dest, const struct lb_str *src, const int *x, const int *y);
int count_str_vischars (int *dest, const struct lb_str *src);

#endif /* LB_FMT_H */
