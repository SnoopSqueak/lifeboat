#ifndef LB_STRING_H
#define LB_STRING_H

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "lb_bool.h"

#define LBF_MAX_STR_LEN 4000
// Large enough to hold most escape sequences, including null character.
#define LBF_ESCSEQ_LEN 12
// Designing for 80x8, not expecting more than 3 digits in a number.
#define LBF_NUMSEQ_LEN 4
#define LBF_NUMBASE 10
#define LBF_NUMBERS "0123456789"
// Character definitions
#define LBF_NULL '\0'
#define LBF_BACKSPACE '\010'
#define LBF_NEWLINE '\012'
#define LBF_RETURN '\015'
#define LBF_ESCAPE '\033'
#define LBF_DELETE '\177'
#define LBF_OSB '['
#define LBF_MINUS '-'
#define LBF_SEMICOLON ';'

// Escape Sequence end codes
#define LBF_ES_UP 'A'
#define LBF_ES_DOWN 'B'
#define LBF_ES_LEFT 'C'
#define LBF_ES_RIGHT 'D'
#define LBF_ES_MOVE 'H'
#define LBF_ES_CLEAR 'J'
#define LBF_ES_GRAPHICS 'm'
#define LBF_ES_ALIGN_LEFT 'L'
#define LBF_ES_ALIGN_CENTER 'C'
#define LBF_ES_ALIGN_RIGHT 'R'

// Custom escape sequences
#define LBF_ALIGN_LEFT "\033[[*;L"
#define LBF_ALIGN_CENTER "\033[[*;C"
#define LBF_ALIGN_RIGHT "\033[[*;R"

// ANSI escape sequences
#define LBF_CLEAR "\033[2J"
#define LBF_RESET "\033[2J\033[1;1H"
#define LBF_UP "\033[A"
#define LBF_DOWN "\033[B"
#define LBF_RIGHT "\033[C"
#define LBF_LEFT "\033[D"

// Foreground colors
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

struct lb_str {
        char *chars;
        int size;
};

int str_from_charcount (struct lb_str **dest, const int *charcount);
int str_from_chars (struct lb_str **dest, const char *src);
int count_str_chars (int *dest, const struct lb_str *src);
int char_from_str (char *dest, const struct lb_str *src, const int *i);
int comp_str_str (int *dest, const struct lb_str *src_a,
                  const struct lb_str *src_b);
int str_ins_str (struct lb_str *dest, const int *di, const struct lb_str *src,
                 const int *si, const int *count);
int str_cat_char (struct lb_str *dest, const char *src);
int str_cat_str (struct lb_str *dest, const struct lb_str *src);
int str_from_int (struct lb_str **dest, const int *src);
int str_del_charcount (struct lb_str *str, const int *i, const int *count);
int str_del_all (struct lb_str *str);
int str_free (struct lb_str **dest);

int str_from_curpos (struct lb_str **dest, const int *x, const int *y);

#endif /* LB_STRING_H */
