#ifndef LB_STRING_H
#define LB_STRING_H

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define LBF_NULL '\0'
#define LBF_BACKSPACE '\010'
#define LBF_NEWLINE '\012'
#define LBF_RETURN '\015'
#define LBF_ESCAPE '\033'
#define LBF_DELETE '\177'
#define LBF_CLEAR "\033[2J"
#define LBF_RESET "\033[2J\033[1;1H"
#define LBF_UP "\033[A"
#define LBF_DOWN "\033[B"
#define LBF_RIGHT "\033[C"
#define LBF_LEFT "\033[D"

#define LBF_ALIGN_LEFT "\033[[*;L"
#define LBF_ALIGN_CENTER "\033[[*;C"
#define LBF_ALIGN_RIGHT "\033[[*;R"

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

int count_string_vis (int * dest, char * source);
int count_string_size (int * dest, char * source);
bool string_is_equal (char * subject, char * target);
int string_ins_char (char * dest, int * index, char * source);
int string_copy (char * dest, int * d_idx, char * source, int * s_idx, int * count);
int string_cat (char * dest, char * source);
int int_from_char (int * dest, char * source);
int int_from_string (int * dest, char * source);
int string_from_int (char * dest, int * source);
int string_move_cur (char * dest, int * x, int * y);

#endif /* LB_STRING_H */
