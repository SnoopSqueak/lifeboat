#ifndef LB_UI_H
#define LB_UI_H

#include "lb_state.h"
#include "lb_sysi/lb_string.h"
#include "lb_ui/lb_tty.h"

//~ struct lb_cmd {
        //~ char * name;
        //~ char ** aliases;
        //~ char * short_desc;
        //~ char ** params_desc;
        //~ int (* handler)(char ** argv);
//~ };

int format_line (struct lb_string * dest, struct lb_state * state, struct lb_string * front, struct lb_string * mid, struct lb_string * end);
int view_landing (struct lb_state * state);

#endif /* LB_UI_H */
