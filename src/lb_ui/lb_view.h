#ifndef LB_UI_H
#define LB_UI_H

#include "lb_sysi/lb_string.h"
#include "lb_ui/lb_tty.h"

//~ struct lb_cmd {
        //~ char * name;
        //~ char ** aliases;
        //~ char * short_desc;
        //~ char ** params_desc;
        //~ int (* handler)(char ** argv);
//~ };

int format_line (char * dest, int * ncol, char * front, char * mid, char * end);
int view_landing (int * ncol, int * nrow, char * swname, char * swvers);

#endif /* LB_UI_H */
