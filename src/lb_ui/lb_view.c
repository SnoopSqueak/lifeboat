#include "lb_view.h"

int format_line (struct lb_string * dest, int ncol, char * front, char * mid, char * end) {
        int flen = 0;
        int mlen = 0;
        int elen = 0;
        struct lb_string * fstr = NULL;
        struct lb_string * mstr = NULL;
        struct lb_string * estr = NULL;
        if (front != NULL) {
                fstr = init_ntstring(front);
                flen += count_vischar(front);
        };
        if (mid != NULL) {
                mstr = init_ntstring(mid);
                mlen += count_vischar(mid);
        };
        if (end != NULL) {
                estr = init_ntstring(end);
                elen += count_vischar(end);
        };
        int len = flen + mlen + elen;
        struct lb_string * tmp;
        if (len > ncol) {
                errno = ERANGE;
                return -1;
        }
        int i = 0;
        int mspace;
        int mpad;
        int mi;
        if (front != NULL) {
                if (put_in_string(dest, -1, fstr) == -1) return -1;
                i += flen;
        };
        if (mid != NULL) {
                if (end != NULL) {
                        mspace = ncol - elen - i;
                } else {
                        mspace = ncol - i;
                };
                mpad = (mspace - mlen) / 2;
                mi = 0;
                tmp = init_ntstring(" ");
                while (mi < mpad) {
                        if (put_in_string(dest, -1, tmp) == -1) return -1;
                        mi++;
                        i++;
                };
                if (put_in_string(dest, -1, mstr) == -1) return -1;
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
                while (i < ncol - elen) {
                        if (put_in_string(dest, -1, tmp) == -1) return -1;
                        i++;
                };
                if (put_in_string(dest, -1, estr) == -1) return -1;
        } else {
                while (i < ncol) {
                        if (put_in_string(dest, -1, tmp) == -1) return -1;
                        i++;
                };
        };
        free_string(&tmp);
        tmp = init_ntstring("\n");
        if (tmp == NULL) return -1;
        if (put_in_string(dest, -1, tmp) == -1) return -1;
        free_string(&tmp);
        if (fstr != NULL) free_string(&fstr);
        if (mstr != NULL) free_string(&mstr);
        if (estr != NULL) free_string(&estr);
        return 0;
};

int view_landing(struct lb_tty * tty, char * swname, char * swvers) {
        if (clear_tty_out(tty) == -1) return -1;
        struct lb_string * lines = init_ntstring("");
        if (lines == NULL) return -1;
        if (format_line(lines, tty->ncol, swname, LBF_FG_BR_BLACK "status: offline" LBF_FG_RESET, swvers) == -1) return -1;
        if (format_line(lines, tty->ncol, "Hosting ## room(s) across ## socket(s).", NULL, "Accepted ## active connection(s).") == -1) return -1;
        if (format_line(lines, tty->ncol, LBF_FG_BR_BLACK "  (## user(s) waiting to connect to you.)" LBF_FG_RESET, NULL, NULL) == -1) return -1;
        if (format_line(lines, tty->ncol, "Joined to ## room(s) across ## socket(s).", NULL, NULL) == -1) return -1;
        if (format_line(lines, tty->ncol, NULL, NULL, NULL) == -1) return -1;
        if (format_line(lines, tty->ncol, LBF_FG_CYAN "Type /help to see available commands." LBF_FG_RESET, NULL, NULL) == -1) return -1;
        if (format_line(lines, tty->ncol, NULL, LBF_FG_BR_BLACK "(notification area)" LBF_FG_RESET, NULL) == -1) return -1;
        if (format_line(lines, tty->ncol, tty->ins->string->ntstring, NULL, NULL) == -1) return -1;
        if (put_to_tty_out(tty, lines) == -1) return -1;
        free_string(&lines);
        return 0;
};
