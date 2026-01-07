#include "lb_string.h"

#define NUMBASE 10
#define ONESLIST "0123456789"
#define MINUSCHAR '-'
#define AS_FIRSTNUM 48

int count_string_vis (int * dest, char * source) {
        int i = 0;
        int count = 0;
        char c = source[i];
        bool escseq = false;
        while (c != LBF_NULL) {
                if (escseq == false) {
                        if (c == LBF_ESCAPE) escseq = true;
                } else {
                        switch (c) {
                                case 'H':
                                case 'J':
                                case 'm':
                                case 'L':
                                case 'C':
                                case 'R':
                                        escseq = false;
                                        count--;
                                break;
                        };
                };
                if (escseq == false) count++;
                i++;
                c = source[i];
        };
        *dest = count;
        return 0;
};

int count_string_size (int * dest, char * source) {
        if (source == NULL || dest == NULL) return -1;
        *dest = 0;
        while (source[*dest] != LBF_NULL) {
                (*dest)++;
        };
        (*dest)++;
        return 0;
};

bool string_is_equal (char * subject, char * target) {
        int i = 0;
        char s;
        char t;
        do {
                s = subject[i];
                t = target[i];
                if (s != t) return false;
                i++;
        } while (s != LBF_NULL && t != LBF_NULL);
        return true;
};

int string_ins_char (char * dest, int * index, char * source) {
        int i = *index;
        char c = dest[i];
        char tmp = dest[i];
        if (c == LBF_NULL) return -1;
        i++;
        while (c != LBF_NULL) {
                tmp = dest[i];
                dest[i] = c;
                i++;
                c = tmp;
        };
        dest[*index] = *source;
        dest[i] = LBF_NULL;
        return 0;
};

int string_copy (char * dest, int * d_idx, char * source, int * s_idx, int * count) {
        int i;
        for (i = 0; i < *count; i++) {
                dest[*d_idx + i] = source[*s_idx + i];
        };
        dest[*d_idx + i] = LBF_NULL;
        return 0;
};

int string_cat (char * dest, char * source) {
        int size;
        if (count_string_size(&size, source) != 0) return -1;
        if (size == 1) return 0;
        int index;
        if (count_string_size(&index, dest) != 0) return -1;
        index--;
        int i = 0;
        char c;
        do {
                c = source[i];
                dest[index + i] = c;
                i++;
        } while (c != LBF_NULL);
        return 0;
};

int string_reverse (char * string) {
        char swp;
        int i = 0;
        int count;
        // TODO: skip nonprinting characters?
        if (count_string_size(&count, string) != 0) return -1;
        count--;
        while (i < count / 2) {
                swp = string[i];
                string[i] = string[count - 1 - i];
                string[count - 1 - i] = swp;
                i++;
        };
        return 0;
};

int int_from_char (int * dest, char * source) {
        int base;
        if (count_string_size(&base, ONESLIST) != 0) return -1;
        if (*source < AS_FIRSTNUM || *source > AS_FIRSTNUM + base) {
                errno = ERANGE;
                return -1;
        };
        *dest = *source - AS_FIRSTNUM;
        return 0;
};

int int_from_string (int * dest, char * source) {
        int total = 0;
        int i;
        if (count_string_size(&i, source) != 0) return -1;
        i -= 2;
        int ival;
        bool isneg = false;
        int exp = 1;
        while (i >= 0) {
                if (i == 0 && source[i] == MINUSCHAR) {
                        isneg = true;
                } else if (int_from_char(&ival, &(source[i])) == -1) {
                        return -1;
                } else {
                        total += ival * exp;
                };
                exp *= NUMBASE;
                i--;
        };
        if (isneg) total = -total;
        *dest = total;
        return 0;
};

int string_from_int (char * dest, int * source) {
        int n = *source;
        if (*source == 0) {
                if (string_cat(dest, "0") != 0) return -1;
                return 0;
        };
        char * tmp = calloc(1, sizeof(char));
        int size = 4;
        int ti = 0;
        bool isneg = n < 0;
        if (isneg) {
                size++;
                n *= -1;
        };
        while (n != 0) {
                if (ti + 1 >= size) {
                        size *= 2;
                        tmp = realloc(tmp, size * sizeof(char));
                };
                tmp[ti] = ONESLIST[n % NUMBASE];
                ti++;
                n = n/10;
        };
        if (isneg) {
                tmp[ti] = MINUSCHAR;
        };
        tmp[ti+1] = LBF_NULL;
        if (string_reverse(tmp) != 0) return -1;
        if (string_cat(dest, tmp) != 0) return -1;
        free(tmp);
        return 0;
};

int string_move_cur (char * dest, int * x, int * y) {
        int tstr_len = 8;
        char * rowstr = calloc(tstr_len, sizeof(char));
        if (string_from_int(rowstr, y) != 0) return -1;
        char * colstr = calloc(tstr_len, sizeof(char));
        if (string_from_int(colstr, x) != 0) return -1;
        char * seq = calloc(3 * tstr_len, sizeof(char));
        if (string_cat(seq, "\033[") != 0) return -1;
        if (string_cat(seq, rowstr) != 0) return -1;
        if (string_cat(seq, ";") != 0) return -1;
        if (string_cat(seq, colstr) != 0) return -1;
        if (string_cat(seq, "H") != 0) return -1;
        if (string_cat(dest, seq) != 0) return -1;
        free(rowstr);
        free(colstr);
        free(seq);
        return 0;
};
