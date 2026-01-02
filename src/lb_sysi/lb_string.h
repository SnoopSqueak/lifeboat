#ifndef LB_STRING_H
#define LB_STRING_H

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define LBF_ESCAPE '\033'
#define LBF_CLEAR "\033[2J"
#define LBF_MOVE_CUR "\033[*;*H"

#define LBF_LEFT "\033[[*;L"
#define LBF_CENTER "\033[[*;C"
#define LBF_RIGHT "\033[[*;R"

#define LBF_FG_BLACK "\033[30m"
#define LBF_FG_RED "\033[31m"
#define LBF_FG_GREEN "\033[32m"
#define LBF_FG_YELLOW "\033[33m"
#define LBF_FG_BLUE "\033[34m"
#define LBF_FG_MAGENTA "\033[35m"
#define LBF_FG_CYAN "\033[36m"
#define LBF_FG_WHITE "\033[37m"
#define LBF_FG_BR_BLACK "\033[90m"
#define LBF_FG_BR_RED "\033[91m"
#define LBF_FG_BR_GREEN "\033[92m"
#define LBF_FG_BR_YELLOW "\033[93m"
#define LBF_FG_BR_BLUE "\033[94m"
#define LBF_FG_BR_MAGENTA "\033[95m"
#define LBF_FG_BR_CYAN "\033[96m"
#define LBF_FG_BR_WHITE "\033[97m"
#define LBF_FG_RESET "\033[0m"

#define NUMBASE 10
#define ONESLIST "0123456789"
#define MINUSCHAR '-'
#define AS_FIRSTNUM 48

struct lb_string {
        char * ntstring;
        int ntsize;
        int maxsize;
};

struct lb_string * init_ntstring (char * ntstring);
struct lb_string * init_utstring (char * utstring, int nchar);
int count_vischar (char * ntstring);
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
bool is_minus_char (char c);
char get_minus_char ();
int free_string (struct lb_string ** str);

#endif /* LB_STRING_H */
