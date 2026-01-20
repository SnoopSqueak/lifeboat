#ifndef LB_UI_H
#define LB_UI_H

#include "lb_sysi/lb_fmt.h"
#include "lb_sysi/lb_str.h"
#include "lb_ui/lb_tty.h"

int view_landing (const int *ncol, const int *nrow, const struct lb_str *swname,
                  const struct lb_str *swvers);

#endif /* LB_UI_H */
