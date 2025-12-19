#ifndef LB_STRING_H
#define LB_STRING_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lb_string {
        char * ntstring;
        size_t ntsize;
};

struct lb_string * init_ntstring (char * ntstring);
struct lb_string * init_utstring (char * utstring, size_t nchar);
int put_in_string (struct lb_string * dest, size_t index, struct lb_string * src);
int take_from_string (struct lb_string * dest, size_t index, ssize_t count);
int string_to_int (struct lb_string * str);
struct lb_string * int_to_string (int n);
int free_string (struct lb_string * str);

#endif /* LB_STRING_H */
