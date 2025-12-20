#include "lb_state.h"

static struct lb_state lbstate;

int lb_state_init (char * lbname, char * lbversion, struct lb_tty * lbtty) {
        lbstate.swname = lbname;
        lbstate.swvers = lbversion;
        lbstate.tty = lbtty;
        return 0;
};

char * lb_name () {
        return lbstate.swname;
};

char * lb_version () {
        return lbstate.swvers;
};

struct lb_tty * get_lb_tty () {
        return lbstate.tty;
};
