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

// TODO: parse AC codes in format (CSI n A)
// CSI is \033[
// n is the color code (or other number)
// A is the control code (m for color)
#define AC_BLACK "\033[30m"
#define AC_RED "\033[31m"
#define AC_GREEN "\033[32m"
#define AC_YELLOW "\033[33m"
#define AC_BLUE "\033[34m"
#define AC_MAGENTA "\033[35m"
#define AC_CYAN "\033[36m"
#define AC_WHITE "\033[37m"
#define AC_BR_BLACK "\033[90m"
#define AC_BR_RED "\033[91m"
#define AC_BR_GREEN "\033[92m"
#define AC_BR_YELLOW "\033[93m"
#define AC_BR_BLUE "\033[94m"
#define AC_BR_MAGENTA "\033[95m"
#define AC_BR_CYAN "\033[96m"
#define AC_BR_WHITE "\033[97m"
#define AC_RESET "\033[0m"

#define AC_UP "\033[1A"
#define AC_DOWN "\033[1B"
#define AC_RIGHT "\033[1C"
#define AC_LEFT "\033[1D"
#define AC_ORIGIN "\033[1;1H"
#define AC_CLEAR "\033[J"
#define AC_NEWLINE "\033[13"

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
bool is_minus_char (char c);
int string_to_int (struct lb_string * str, int * dest);
struct lb_string * int_to_string (int n);
int free_string (struct lb_string ** str);

#endif /* LB_STRING_H */
