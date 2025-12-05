#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// AC = ANSI colors
#define AC_BLACK "\x1b[30m"
#define AC_RED "\x1b[31m"
#define AC_GREEN "\x1b[32m"
#define AC_YELLOW "\x1b[33m"
#define AC_BLUE "\x1b[34m"
#define AC_MAGENTA "\x1b[35m"
#define AC_CYAN "\x1b[36m"
#define AC_WHITE "\x1b[37m"
#define AC_BR_BLACK "\x1b[90m"
#define AC_BR_RED "\x1b[91m"
#define AC_BR_GREEN "\x1b[92m"
#define AC_BR_YELLOW "\x1b[93m"
#define AC_BR_BLUE "\x1b[94m"
#define AC_BR_MAGENTA "\x1b[95m"
#define AC_BR_CYAN "\x1b[96m"
#define AC_BR_WHITE "\x1b[97m"
#define AC_RESET "\x1b[0m"

// LL = log level
// The logger will log a given message if and only if that message's 
//      log level is equal to or greater than the logger's log level.
// Setting the logger to LL_NEVER means it will log everything it can.
#define LL_NEVER 0
#define LL_TRACE 1
#define LL_DEBUG 2
#define LL_INFO 3
#define LL_WARN 4
#define LL_ERROR 5
#define LL_ALWAYS 6

#define CF_NONE 0
#define CF_ANSI 1

struct logger {
        int log_level;
        FILE * out_dest;
        int color_flag;
};

struct logger * make_logger(int log_level, FILE * out_dest, int color_flag);

int log_this(struct logger * dest_logger, char * message, int msg_log_lvl, ...);

#endif /* LOGGER_H */
