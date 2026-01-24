#ifndef LB_STRING_H
#define LB_STRING_H

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "lb_bool.h"

#define LBF_MAX_STR_LEN 65534
// Large enough to hold most escape sequences, including null character.
#define LBF_ESCSEQ_LEN 12
// Designing for 80x8, not expecting more than 3 digits in a number.
#define LBF_NUMSEQ_LEN 4
#define LBF_NUMBASE 10
#define LBF_NUMBERS "0123456789"
// Character definitions
#define LBF_NULL '\0'
#define LBF_BACKSPACE '\010'
// #define LBF_NEWLINE '\012'
#define LBF_RETURN '\015'
#define LBF_MINUS '-'

struct lb_str {
        char *chars;
        unsigned int size;
};

int init_str_charcount (struct lb_str **dest, const unsigned int *charcount);
int init_str_str (struct lb_str **dest, const struct lb_str *src);
int init_str_chars (struct lb_str **dest, const char *src);
int init_str_int (struct lb_str **dest, const int *src);
int count_str_chars (int *dest, const struct lb_str *src);
int char_from_str (char *dest, const struct lb_str *src, const int *i);
int comp_chars_chars (int *dest, const char *src_a, const char *src_b);
int comp_str_str (int *dest, const struct lb_str *src_a, const struct lb_str *src_b);
int str_ins_str (struct lb_str *dest, const int *di, const struct lb_str *src,
                 const int *si, const int *count);
int str_cat_char (struct lb_str *dest, const char *src);
int str_cat_chars (struct lb_str *dest, const char *src);
int str_cat_str (struct lb_str *dest, const struct lb_str *src);
int str_del_charcount (struct lb_str *dest, const int *i, const int *count);
int str_del_all (struct lb_str *dest);
int free_str (struct lb_str **dest);

#endif /* LB_STRING_H */
