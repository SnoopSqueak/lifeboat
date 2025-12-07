#include "logger.h"

struct logger * make_logger(int log_level, FILE * out_dest, int color_flag) {
        struct logger * new_logger = malloc(sizeof(*new_logger));
        new_logger->log_level = log_level;
        new_logger->out_dest = out_dest;
        new_logger->color_flag = color_flag;
        return new_logger;
}

void free_logger(struct logger * old_logger) {
        free(old_logger);
}

int log_this (struct logger * dest_logger, char * message, int msg_log_lvl, ...) {
        if (
                dest_logger == NULL ||
                msg_log_lvl == LL_NEVER ||
                dest_logger->log_level == LL_ALWAYS
        ) {
                return 0;
        }
        if (dest_logger->log_level <= msg_log_lvl) {
                va_list args;
                va_start(args, msg_log_lvl);
                // TODO: if color flag is NONE, strip out color characters.
                if (dest_logger->color_flag == CF_ANSI) {
                        char * color;
                        switch (msg_log_lvl) {
                                case LL_ALWAYS:
                                        color = AC_BR_WHITE;
                                        break;
                                case LL_TRACE:
                                        color = AC_BR_GREEN;
                                        break;
                                case LL_DEBUG:
                                        color = AC_BR_BLACK;
                                        break;
                                case LL_INFO:
                                        color = AC_WHITE;
                                        break;
                                case LL_WARN:
                                        color = AC_BR_YELLOW;
                                        break;
                                case LL_ERROR:
                                        color = AC_BR_RED;
                                        break;
                        }
                        fprintf(dest_logger->out_dest, color);
                }
                vfprintf(dest_logger->out_dest, message, args);
                if (dest_logger->color_flag == CF_ANSI) {
                        fprintf(dest_logger->out_dest, AC_RESET);
                }
                va_end(args);
        }
        return 0;
}
