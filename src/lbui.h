#ifndef LBUI_H
#define LBUI_H

struct command {
        char * name;
        char ** aliases;
        char * short_desc;
        char ** params_desc;
        int (* handler)(char ** argv);
};

//int parse_input

#endif
