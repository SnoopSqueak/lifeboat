#include "lb_ui.h"

int lb_line (char * dest, size_t row, char * front, char * mid, char * end) {
        size_t flen = 0;
        size_t mlen = 0;
        size_t elen = 0;
        if (front != NULL) flen = strlen(front);
        if (mid != NULL) mlen += strlen(mid);
        if (end != NULL) elen += strlen(end);
        size_t len = flen + mlen + elen;
        size_t cc = get_lb_tty()->ncol;
        size_t rc = get_lb_tty()->nrow;
        if (len > cc) {
                errno = ERANGE;
                return -1;
        }
        size_t i = 0;
        size_t di = (cc+1) * row;
        size_t mspace;
        size_t mpad;
        size_t mi;
        if (front != NULL) {
                while (i < flen) {
                        dest[di+i] = front[i];
                        i++;
                }
        }
        if (mid != NULL) {
                if (end != NULL) {
                        mspace = cc - elen - i;
                } else {
                        mspace = cc - i;
                }
                mpad = (mspace - mlen)/2;
                mi = 0;
                while (mi < mpad) {
                        dest[di+i] = ' ';
                        mi++;
                        i++;
                }
                mi = 0;
                while (mi < mlen) {
                        dest[di+i] = mid[mi];
                        mi++;
                        i++;
                }
                mi = 0;
                while (mi < mpad) {
                        dest[di+i] = ' ';
                        mi++;
                        i++;
                }
        }
        if (end != NULL) {
                while (i < cc) {
                        if (i < cc - elen) {
                                dest[di+i] = ' ';
                        } else {
                                dest[di+i] = end[i - cc + elen];
                        }
                        i++;
                }
        }
        if (len == 0) {
                while (i < cc) {
                        dest[di+i] = ' ';
                        i++;
                }
        }
        if (row == rc - 1) {
                dest[di+i] = '\0';
        } else {
                dest[di+i] = '\n';
        }
        return 0;
}

int lb_view_landing(struct lb_tty * tty) {
        int errors = 0;
        clear_tty_out(tty);
        //~ size_t fullsize = (tty->ncol + 1) * tty->nrow * sizeof(char);
        struct lb_string * lines = init_ntstring("This is a test.");
        struct lb_string * left = init_ntstring(LB_LEFT);
        //char * lines = calloc((tty->ncol + 1) * tty->nrow, sizeof(char));
        //~ errors += lb_line(lines, 0, lb_name(), AC_BR_BLACK "status: offline" AC_RESET, lb_version());
        //~ errors += lb_line(lines, 1, "Hosting ## room(s) across ## socket(s).", NULL, "Accepted ## active connection(s).");
        //~ errors += lb_line(lines, 2, AC_BR_BLACK "  (## user(s) waiting to connect to you.)" AC_RESET, NULL, NULL);
        //~ errors += lb_line(lines, 3, "Joined to ## room(s) across ## socket(s).", NULL, NULL);
        //~ errors += lb_line(lines, 4, NULL, NULL, NULL);
        //~ errors += lb_line(lines, 5, AC_CYAN "Type /help to see available commands." AC_RESET, NULL, NULL);
        //~ errors += lb_line(lines, 6, NULL, AC_BR_BLACK "(notification area)" AC_RESET, NULL);
        //~ errors += lb_line(lines, 7, tty->ins->string->ntstring, NULL, NULL);
        if (errors == 0) put_to_tty_out(tty, lines);
        //~ for (size_t i = 0; i < tty->ncol; i++) put_to_tty_out(tty, left);
        free(lines);
        free(left);
        return errors;
}
