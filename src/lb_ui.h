#ifndef LB_UI_H
#define LB_UI_H

#include "lb_state.h"
#include "lb_term.h"

#define AC_BLACK "\x1b[30m"
#define AC_RED "\x1b[31m"
#define AC_GREEN "\x1b[32m"
#define AC_YELLOW "\x1b[33m"
#define AC_BLUE "\x1b[34m"
#define AC_MAGENTA "\x1b[35m"
#define AC_CYAN "\x1b[36m"
#define AC_WHITE "\x1b[37m"
#define AC_BR_BLACK "\x1b[90m"
#define AC_BR_RED "\x1b[91m"
#define AC_BR_GREEN "\x1b[92m"
#define AC_BR_YELLOW "\x1b[93m"
#define AC_BR_BLUE "\x1b[94m"
#define AC_BR_MAGENTA "\x1b[95m"
#define AC_BR_CYAN "\x1b[96m"
#define AC_BR_WHITE "\x1b[97m"
#define AC_RESET "\x1b[0m"

//~ struct lb_cmd {
        //~ char * name;
        //~ char ** aliases;
        //~ char * short_desc;
        //~ char ** params_desc;
        //~ int (* handler)(char ** argv);
//~ };

int lb_ui_init(struct lb_state * state);
int lb_line (char * dest, size_t row, char * front, char * mid, char * end);
int lb_view_landing();

#endif /* LB_UI_H */
