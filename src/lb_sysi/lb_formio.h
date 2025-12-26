#ifndef LB_FORMIO_H
#define LB_FORMIO_H

#include <stdbool.h>
#include <stddef.h>

#define NUMBASE 10
#define ONESLIST "0123456789"
#define MINUSCHAR '-'
#define AS_FIRSTNUM 48

//~ #define AS_BACKSPACE 8
//~ #define AS_NEWLINE 10
//~ #define AS_RETURN 13
//~ #define AS_ESCAPE 27
//~ #define AS_A 65
//~ #define AS_B 66
//~ #define AS_C 67
//~ #define AS_D 68
//~ #define AS_OSB 91
//~ #define AS_DELETE 127
//~ #define AS_UP AS_A
//~ #define AS_DOWN AS_B
//~ #define AS_RIGHT AS_C
//~ #define AS_LEFT AS_D


#define AC_ESC "\033"

#define AC_OSB "["

#define AC_BLACK_CODE "30"
#define AC_RED_CODE "31"
#define AC_GREEN_CODE "32"
#define AC_YELLOW_CODE "33"
#define AC_BLUE_CODE "34"
#define AC_MAGENTA_CODE "35"
#define AC_CYAN_CODE "36"
#define AC_WHITE_CODE "37"
#define AC_BR_BLACK_CODE "90"
#define AC_BR_RED_CODE "91"
#define AC_BR_GREEN_CODE "92"
#define AC_BR_YELLOW_CODE "93"
#define AC_BR_BLUE_CODE "94"
#define AC_BR_MAGENTA_CODE "95"
#define AC_BR_CYAN_CODE "96"
#define AC_BR_WHITE_CODE "97"
#define AC_RESET_CODE "0"
#define AC_COLOR_CODE "m"

#define AC_DIR_CODE "1"
#define AC_UP_CODE "A"
#define AC_DOWN_CODE "B"
#define AC_RIGHT_CODE "C"
#define AC_LEFT_CODE "D"
#define AC_ORIGIN_CODE "H" // can take two arguments
#define AC_CLEAR_CODE "J"
#define AC_NEWLINE_CODE "13"

//~ int count_vischar (char * ntstring);
bool is_minus_char (char c);
char get_minus_char ();
char * home_and_clear ();

#endif /* LB_FORMIO_H */
