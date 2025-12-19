#ifndef LB_STATE_H
#define LB_STATE_H

#include "lb_term.h"

struct lb_state {
        char * swname;
        char * swvers;
        struct lb_term * term;
};

int lb_state_init (char * lbname, char * lbversion, struct lb_term * lbterm);
char * lb_name ();
char * lb_version ();
struct lb_term * get_lb_term ();

#endif /* LB_STATE_H */
