#ifndef LB_STATE_H
#define LB_STATE_H

#include "lb_ui/lb_tty.h"

struct lb_state {
        char * swname;
        char * swvers;
        struct lb_tty * tty;
        int chid;
};

#endif /* LB_STATE_H */
