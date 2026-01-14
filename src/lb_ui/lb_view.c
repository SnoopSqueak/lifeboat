#include "lb_view.h"

// int format_line (struct lb_str *dest, int *ncol, struct lb_str *beg, struct lb_str *mid, struct lb_str *end) {
//         int blen = 0, mlen = 0, elen = 0;
//         struct lb_str *bstr, *mstr, *estr, *tmp;
//         if (str_from_)
//
//         if (beg != NULL) {
//                 if (string_cat(bstr, beg) != 0) return -1;
//                 if (string_count_vis(&blen, beg) != 0) return -1;
//         };
//         if (mid != NULL) {
//                 if (string_cat(mstr, mid) != 0) return -1;
//                 if (string_count_vis(&mlen, mid) != 0) return -1;
//         };
//         if (end != NULL) {
//                 if (string_cat(estr, end) != 0) return -1;
//                 if (string_count_vis(&elen, end) != 0) return -1;
//         };
//         int len = blen + mlen + elen;
//         if (len > *ncol) {
//                 errno = ERANGE;
//                 return -1;
//         };
//         int i = 0;
//         int mspace;
//         int mpad;
//         int mi;
//         if (beg != NULL) {
//                 if (string_cat(dest, bstr) != 0) return -1;
//                 i += blen;
//         };
//         if (mid != NULL) {
//                 if (end != NULL) {
//                         mspace = *ncol - elen - i;
//                 } else {
//                         mspace = *ncol - i;
//                 };
//                 mpad = (mspace - mlen) / 2;
//                 mi = 0;
//                 string_cat(tmp, " ");
//                 while (mi < mpad) {
//                         if (string_cat(dest, tmp) != 0) return -1;
//                         mi++;
//                         i++;
//                 };
//                 if (string_cat(dest, mstr) != 0) return -1;
//                 i += mlen;
//                 mi = 0;
//                 while (mi < mpad) {
//                         if (string_cat(dest, tmp) == -1) return -1;
//                         mi++;
//                         i++;
//                 };
//                 tmp[0] = LBF_NULL;
//         };
//         string_cat(tmp, " ");
//         if (tmp == NULL) return -1;
//         if (end != NULL) {
//                 while (i < *ncol - elen) {
//                         if (string_cat(dest, tmp) != 0) return -1;
//                         i++;
//                 };
//                 if (string_cat(dest, estr) != 0) return -1;
//         } else {
//                 while (i < *ncol) {
//                         if (string_cat(dest, tmp) != 0) return -1;
//                         i++;
//                 };
//         };
//         tmp[0] = LBF_NEWLINE;
//         tmp[1] = LBF_NULL;
//         if (string_cat(dest, tmp) != 0) return -1;
//         free(tmp);
//         free(bstr);
//         free(mstr);
//         free(estr);
//         return 0;
// };

int view_landing(int *ncol, int *nrow, struct lb_str *swname,
                 struct lb_str *swvers) {
        // Doubling space, for nonvisible characters
        int lsize = (*ncol) * (*nrow) * 2;
        int defsize = 4;
        struct lb_str *lines, *beg, *mid, *end;
        if (str_from_charcount(&lines, &lsize) != 0) return -1;
        if (str_from_charcount(&beg, &defsize) != 0) goto cleanlines;

        if (str_from_chars(&mid, "This is a test!") != 0) goto cleanbeg;
        if (fmt_newline(mid) != 0) goto cleanmid;
        // if (str_from_charcount(&mid, &defsize) != 0) goto cleanbeg;
        if (str_from_charcount(&end, &defsize) != 0) goto cleanmid;
        if (str_cat_str(beg, swname) != 0) goto cleanend;
        if (str_cat_str(end, swvers) != 0) goto cleanend;
        //if (format_line(lines, ncol, beg, mid, end) != 0) goto cleantmp;
        if (str_cat_str(lines, mid) != 0) goto cleanend;

        if (str_cat_str(lines, mid) != 0) goto cleanend;
        if (str_cat_str(lines, mid) != 0) goto cleanend;
        if (str_cat_str(lines, mid) != 0) goto cleanend;
        if (str_cat_str(lines, mid) != 0) goto cleanend;
        if (str_cat_str(lines, mid) != 0) goto cleanend;
        if (str_cat_str(lines, mid) != 0) goto cleanend;
        if (str_cat_str(lines, mid) != 0) goto cleanend;
        if (str_cat_str(lines, mid) != 0) goto cleanend;
        if (str_cat_str(lines, mid) != 0) goto cleanend;
        if (str_cat_str(lines, mid) != 0) goto cleanend;

        if (free_str(&end) != 0) goto cleanmid;
        if (free_str(&mid) != 0) goto cleanbeg;
        if (free_str(&beg) != 0) goto cleanlines;
        // if (format_line(lines, ncol, swname,
        //         LBF_FG_BR_BLACK "status: offline" LBF_FG_RESET, swvers) == -1)
        //         return -1;
        // if (format_line(lines, ncol,
        //         "Hosting ## room(s) across ## socket(s).", NULL,
        //                 "Accepted ## active connection(s).") == -1) return -1;
        // if (format_line(lines, ncol,
        //         LBF_FG_BR_BLACK "  (## user(s) waiting to connect to you.)"
        //         LBF_FG_RESET, NULL, NULL) == -1) return -1;
        // if (format_line(lines, ncol,
        //         "Joined to ## room(s) across ## socket(s).", NULL, NULL)
        //         == -1) return -1;
        // if (format_line(lines, ncol, NULL, NULL, NULL) == -1) return -1;
        // if (format_line(lines, ncol, LBF_FG_CYAN
        //         "Type /help to see available commands." LBF_FG_RESET,
        //         NULL, NULL) == -1) return -1;
        // if (format_line(lines, ncol, NULL,
        //         LBF_FG_BR_BLACK "(notification area)" LBF_FG_RESET, NULL)
        //         == -1) return -1;
        if (tty_put_out(lines) != 0) goto cleanlines;
        free_str(&lines);
        return 0;
cleanend:
        free_str(&end);
cleanmid:
        free_str(&mid);
cleanbeg:
        free_str(&beg);
cleanlines:
        free_str(&lines);
        return -1;
};
