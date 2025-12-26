#include "lb_view.h"

int format_line (struct lb_string * dest, struct lb_state * state, struct lb_string * front, struct lb_string * mid, struct lb_string * end) {
        int flen = 0;
        int mlen = 0;
        int elen = 0;
        if (front != NULL) flen = front->ntsize - 1;
        if (mid != NULL) mlen += mid->ntsize - 1;
        if (end != NULL) elen += end->ntsize - 1;
        int len = flen + mlen + elen;
        struct lb_string * tmp;
        int cc = state->tty->ncol;
        if (len > cc) {
                errno = ERANGE;
                return -1;
        }
        int i = 0;
        int mspace;
        int mpad;
        int mi;
        if (front != NULL) {
                if (put_in_string(dest, -1, front) == -1) return -1;
                i += flen;
        };
        if (mid != NULL) {
                if (end != NULL) {
                        mspace = cc - elen - i;
                } else {
                        mspace = cc - i;
                };
                mpad = (mspace - mlen) / 2;
                mi = 0;
                tmp = init_ntstring(" ");
                while (mi < mpad) {
                        if (put_in_string(dest, -1, tmp) == -1) return -1;
                        mi++;
                        i++;
                };
                if (put_in_string(dest, -1, mid) == -1) return -1;
                i += mlen;
                mi = 0;
                while (mi < mpad) {
                        if (put_in_string(dest, -1, tmp) == -1) return -1;
                        mi++;
                        i++;
                };
                free_string(&tmp);
        };
        tmp = init_ntstring(" ");
        if (tmp == NULL) return -1;
        if (end != NULL) {
                while (i < cc - elen) {
                        if (put_in_string(dest, -1, tmp) == -1) return -1;
                        i++;
                };
                if (put_in_string(dest, -1, end) == -1) return -1;
        } else {
                while (i < cc) {
                        if (put_in_string(dest, -1, tmp) == -1) return -1;
                        i++;
                };
        };
        free_string(&tmp);
        tmp = init_ntstring("\n");
        if (tmp == NULL) return -1;
        if (put_in_string(dest, -1, tmp) == -1) return -1;
        free_string(&tmp);
        return 0;
};

int view_landing(struct lb_state * state) {
        clear_tty_out(state->tty);
        struct lb_string * lines = init_ntstring("");
        //~ struct lb_string * left = init_ntstring(LB_LEFT);
        struct lb_string * front = init_ntstring(state->swname);
        // invisible/control characters still count against total...
        //~ struct lb_string * mid = init_ntstring(AC_BR_BLACK "status: offline" AC_RESET);
        struct lb_string * mid = init_ntstring("status: offline");
        struct lb_string * end = init_ntstring(state->swvers);
        if (lines == NULL || front == NULL || mid == NULL || end == NULL) return -1;
        if (format_line(lines, state, front, mid, end) == -1) return -1;
        if (put_to_tty_out(state->tty, lines) == -1) return -1;
        free_string(&front);
        free_string(&mid);
        free_string(&end);
        free_string(&lines);
        
        //char * lines = calloc((tty->ncol + 1) * tty->nrow, sizeof(char));
        //~ errors += lb_line(lines, 0, lb_name(), AC_BR_BLACK "status: offline" AC_RESET, lb_version());
        //~ errors += lb_line(lines, 1, "Hosting ## room(s) across ## socket(s).", NULL, "Accepted ## active connection(s).");
        //~ errors += lb_line(lines, 2, AC_BR_BLACK "  (## user(s) waiting to connect to you.)" AC_RESET, NULL, NULL);
        //~ errors += lb_line(lines, 3, "Joined to ## room(s) across ## socket(s).", NULL, NULL);
        //~ errors += lb_line(lines, 4, NULL, NULL, NULL);
        //~ errors += lb_line(lines, 5, AC_CYAN "Type /help to see available commands." AC_RESET, NULL, NULL);
        //~ errors += lb_line(lines, 6, NULL, AC_BR_BLACK "(notification area)" AC_RESET, NULL);
        //~ errors += lb_line(lines, 7, tty->ins->string->ntstring, NULL, NULL);
        //~ if (errors == 0) put_to_tty_out(tty, lines);
        //~ for (size_t i = 0; i < tty->ncol; i++) put_to_tty_out(tty, left);
        //~ free(lines);
        //~ free(left);
        return 0;
};
