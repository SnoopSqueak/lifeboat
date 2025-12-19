#include "lb_state.h"

static struct lb_state lbstate;

int lb_state_init (char * lbname, char * lbversion, struct lb_term * lbterm) {
        lbstate.swname = lbname;
        lbstate.swvers = lbversion;
        lbstate.term = lbterm;
        return 0;
};

char * lb_name () {
        return lbstate.swname;
};

char * lb_version () {
        return lbstate.swvers;
};

struct lb_term * get_lb_term () {
        return lbstate.term;
};
