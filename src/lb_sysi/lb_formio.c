#include "lb_formio.h"

//~ int count_vischar (char * ntstring) {
        //~ if (ntstring[0] == '\0') return -1;
        //~ int i = 0;
        //~ char c = ntstring[i];
        //~ while (c != '\0') {
                //~ i++;
                //~ c = ntstring[i];
        //~ };
        //~ return i;
//~ };

bool is_minus_char (char c) {
        return (c == get_minus_char());
};

char get_minus_char () {
        return MINUSCHAR;
};

char * home_and_clear () {
        return AC_ESC AC_OSB AC_CLEAR_CODE AC_ESC AC_OSB "1;1;" AC_ORIGIN_CODE;
};
