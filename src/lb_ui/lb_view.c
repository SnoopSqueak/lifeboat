#include "lb_view.h"

int format_line (char *dest, int *ncol, char *front, char *mid, char *end) {
        int flen = 0;
        int mlen = 0;
        int elen = 0;
        char *tmp = calloc((*ncol) * 2, sizeof(char));
        char *fstr = calloc((*ncol) * 2, sizeof(char));
        char *mstr = calloc((*ncol) * 2, sizeof(char));
        char *estr = calloc((*ncol) * 2, sizeof(char));
        if (tmp == NULL || fstr == NULL || mstr == NULL || estr == NULL)
                return -1;
        if (front != NULL) {
                if (string_cat(fstr, front) != 0) return -1;
                if (string_count_vis(&flen, front) != 0) return -1;
        };
        if (mid != NULL) {
                if (string_cat(mstr, mid) != 0) return -1;
                if (string_count_vis(&mlen, mid) != 0) return -1;
        };
        if (end != NULL) {
                if (string_cat(estr, end) != 0) return -1;
                if (string_count_vis(&elen, end) != 0) return -1;
        };
        int len = flen + mlen + elen;
        if (len > *ncol) {
                errno = ERANGE;
                return -1;
        };
        int i = 0;
        int mspace;
        int mpad;
        int mi;
        if (front != NULL) {
                if (string_cat(dest, fstr) != 0) return -1;
                i += flen;
        };
        if (mid != NULL) {
                if (end != NULL) {
                        mspace = *ncol - elen - i;
                } else {
                        mspace = *ncol - i;
                };
                mpad = (mspace - mlen) / 2;
                mi = 0;
                string_cat(tmp, " ");
                while (mi < mpad) {
                        if (string_cat(dest, tmp) != 0) return -1;
                        mi++;
                        i++;
                };
                if (string_cat(dest, mstr) != 0) return -1;
                i += mlen;
                mi = 0;
                while (mi < mpad) {
                        if (string_cat(dest, tmp) == -1) return -1;
                        mi++;
                        i++;
                };
                tmp[0] = LBF_NULL;
        };
        string_cat(tmp, " ");
        if (tmp == NULL) return -1;
        if (end != NULL) {
                while (i < *ncol - elen) {
                        if (string_cat(dest, tmp) != 0) return -1;
                        i++;
                };
                if (string_cat(dest, estr) != 0) return -1;
        } else {
                while (i < *ncol) {
                        if (string_cat(dest, tmp) != 0) return -1;
                        i++;
                };
        };
        tmp[0] = LBF_NEWLINE;
        tmp[1] = LBF_NULL;
        if (string_cat(dest, tmp) != 0) return -1;
        free(tmp);
        free(fstr);
        free(mstr);
        free(estr);
        return 0;
};

int view_landing(int *ncol, int *nrow, char *swname, char *swvers) {
        // Doubling space, for nonvisible characters
        char *lines = calloc((*ncol) * (*nrow) * 2, sizeof(char));
        if (lines == NULL) return -1;
        if (format_line(lines, ncol, swname,
                LBF_FG_BR_BLACK "status: offline" LBF_FG_RESET, swvers) == -1)
                return -1;
        if (format_line(lines, ncol,
                "Hosting ## room(s) across ## socket(s).", NULL,
                        "Accepted ## active connection(s).") == -1) return -1;
        if (format_line(lines, ncol,
                LBF_FG_BR_BLACK "  (## user(s) waiting to connect to you.)"
                LBF_FG_RESET, NULL, NULL) == -1) return -1;
        if (format_line(lines, ncol,
                "Joined to ## room(s) across ## socket(s).", NULL, NULL)
                == -1) return -1;
        if (format_line(lines, ncol, NULL, NULL, NULL) == -1) return -1;
        if (format_line(lines, ncol, LBF_FG_CYAN
                "Type /help to see available commands." LBF_FG_RESET,
                NULL, NULL) == -1) return -1;
        if (format_line(lines, ncol, NULL,
                LBF_FG_BR_BLACK "(notification area)" LBF_FG_RESET, NULL)
                == -1) return -1;
        if (tty_put_out(lines) != 0) return -1;
        free(lines);
        return 0;
};
