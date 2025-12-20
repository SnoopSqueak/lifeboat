#ifndef LB_STRING_H
#define LB_STRING_H

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBASE 10
#define ONESLIST "0123456789"
#define AS_FIRSTNUM 48

struct lb_string {
        char * ntstring;
        size_t ntsize;
};

struct lb_string * init_ntstring (char * ntstring);
struct lb_string * init_utstring (char * utstring, size_t nchar);
ssize_t count_ntstring (char * ntstring);
int grow_string (struct lb_string * str, size_t new_size);
int put_in_string (struct lb_string * dest, size_t index, struct lb_string * src);
int take_from_string (struct lb_string * dest, size_t index, ssize_t count);
int char_to_int (char c, int * dest);
bool is_minus_char (char c);
int string_to_int (struct lb_string * str, int * dest);
struct lb_string * int_to_string (int n);
int reverse_string (struct lb_string * str);
int free_string (struct lb_string * str);

#endif /* LB_STRING_H */
