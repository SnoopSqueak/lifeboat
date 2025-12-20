#ifndef LB_STRING_H
#define LB_STRING_H

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBASE 10
#define ONESLIST "0123456789"
#define MINUSSTRING "-"

#define AS_FIRSTNUM 48
#define AS_BACKSPACE 8
#define AS_NEWLINE 10
#define AS_RETURN 13
#define AS_ESCAPE 27
#define AS_A 65
#define AS_B 66
#define AS_C 67
#define AS_D 68
#define AS_OSB 91
#define AS_DELETE 127
#define AS_UP AS_A
#define AS_DOWN AS_B
#define AS_RIGHT AS_C
#define AS_LEFT AS_D

#define LB_UP "\033[1A"
#define LB_DOWN "\033[1B"
#define LB_RIGHT "\033[1C"
#define LB_LEFT "\033[1D"
#define LB_ORIGIN "\033[1;1H"
#define LB_CLEAR "\033[J"
#define LB_NEWLINE "\033[13"

struct lb_string {
        char * ntstring;
        size_t ntsize;
        ssize_t maxsize;
};

struct lb_string * init_ntstring (char * ntstring);
struct lb_string * init_utstring (char * utstring, size_t nchar);
ssize_t count_ntstring (char * ntstring);
int grow_string (struct lb_string * str, size_t new_size);
int shrink_string (struct lb_string * dest, size_t newsize);
int put_in_string (struct lb_string * dest, ssize_t index, struct lb_string * src);
int take_from_string (struct lb_string * dest, size_t index, ssize_t count);
int clear_string (struct lb_string * string);
int char_to_int (char c, int * dest);
bool is_minus_char (char c);
int string_to_int (struct lb_string * str, int * dest);
struct lb_string * int_to_string (int n);
int reverse_string (struct lb_string * str);
int free_string (struct lb_string * str);

#endif /* LB_STRING_H */
