#ifndef LB_STATE_H
#define LB_STATE_H

#include "lb_tty.h"

struct lb_state {
        char * swname;
        char * swvers;
        struct lb_tty * tty;
};

int lb_state_init (char * lbname, char * lbversion, struct lb_tty * lbtty);
char * lb_name ();
char * lb_version ();
struct lb_tty * get_lb_tty ();

#endif /* LB_STATE_H */
