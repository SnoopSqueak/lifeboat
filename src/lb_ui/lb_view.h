#ifndef LB_UI_H
#define LB_UI_H

#include "lb_sysi/lb_fmt.h"
#include "lb_sysi/lb_str.h"
#include "lb_ui/lb_tty.h"

//~ struct lb_cmd {
        //~ char * name;
        //~ char ** aliases;
        //~ char * short_desc;
        //~ char ** params_desc;
        //~ int (* handler)(char ** argv);
//~ };

int view_landing (int *ncol, int *nrow, struct lb_str *swname,
                  struct lb_str *swvers);

#endif /* LB_UI_H */
