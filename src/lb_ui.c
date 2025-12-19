#include "lb_ui.h"

static struct lb_state * lbstate;

int lb_ui_init (struct lb_state * state) {
        lbstate = state;
        return 0;
};

int lb_line (char * dest, size_t row, char * front, char * mid, char * end) {
        size_t flen = 0;
        size_t mlen = 0;
        size_t elen = 0;
        if (front != NULL) flen = strlen(front);
        if (mid != NULL) mlen += strlen(mid);
        if (end != NULL) elen += strlen(end);
        size_t len = flen + mlen + elen;
        size_t cc = get_lb_term()->colcount;
        size_t rc = get_lb_term()->rowcount;
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

int lb_view_landing() {
        int errors = 0;
        clearterm(get_lb_term());
        char * lines = calloc((get_lb_term()->colcount + 1) * get_lb_term()->rowcount, sizeof(char));
        errors += lb_line(lines, 0, lb_name(), AC_BR_BLACK "status: offline" AC_RESET, lb_version());
        errors += lb_line(lines, 1, "Hosting ## room(s) across ## socket(s).", NULL, "Accepted ## active connection(s).");
        errors += lb_line(lines, 2, AC_BR_BLACK "  (## user(s) waiting to connect to you.)" AC_RESET, NULL, NULL);
        errors += lb_line(lines, 3, "Joined to ## room(s) across ## socket(s).", NULL, NULL);
        errors += lb_line(lines, 4, NULL, NULL, NULL);
        errors += lb_line(lines, 5, AC_CYAN "Type /help to see available commands." AC_RESET, NULL, NULL);
        errors += lb_line(lines, 6, NULL, AC_BR_BLACK "(notification area)" AC_RESET, NULL);
        //errors += lb_line(lines, 7, AC_RESET "usr input go here", NULL, NULL);
        errors += lb_line(lines, 7, get_lb_term()->inbuf, NULL, NULL);
        if (errors == 0) print_lines(get_lb_term(), lines);
        free(lines);
        for (size_t i = 0; i < get_lb_term()->colcount; i++) print_to_term(get_lb_term(), LB_LEFT, 4);
        return errors;
}
