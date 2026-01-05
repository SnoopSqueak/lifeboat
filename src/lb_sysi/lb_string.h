#ifndef LB_STRING_H
#define LB_STRING_H

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define LBF_NEWLINE '\012'
#define LBF_RETURN '\015'
#define LBF_ESCAPE '\033'
#define LBF_CLEAR "\033[2J"
#define LBF_RESET "\033[2J\033[1;1H"

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

int count_string_vis (int * dest, char * source);
int string_cat (char * dest, char * source);
int int_from_char (int * dest, char * source);
int int_from_string (int * dest, char * source);
int string_from_int (char * dest, int * source);

#endif /* LB_STRING_H */
