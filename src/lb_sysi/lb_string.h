#ifndef LB_STRING_H
#define LB_STRING_H

#include "lb_formio.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

struct lb_string {
        char * ntstring;
        int ntsize;
        int maxsize;
};

struct lb_string * init_ntstring (char * ntstring);
struct lb_string * init_utstring (char * utstring, int nchar);
int count_ntstring (char * ntstring);
int grow_string (struct lb_string * str, int new_size);
int shrink_string (struct lb_string * dest, int newsize);
int put_in_string (struct lb_string * dest, int index, struct lb_string * src);
int take_from_string (struct lb_string * dest, int index, int count);
int clear_string (struct lb_string * string);
int reverse_string (struct lb_string * str);
int char_to_int (char c, int * dest);
int string_to_int (struct lb_string * str, int * dest);
struct lb_string * int_to_string (int n);
int free_string (struct lb_string ** str);

#endif /* LB_STRING_H */
