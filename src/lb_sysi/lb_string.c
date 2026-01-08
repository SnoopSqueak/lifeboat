#include "lb_string.h"

#define AS_FIRSTNUM 48

int string_count_vis (int * dest, char * source) {
        int i = 0;
        int count = 0;
        char c = source[i];
        bool escseq = false;
        while (c != LBF_NULL) {
                if (escseq == false) {
                        if (c == LBF_ESCAPE) escseq = true;
                } else {
                        switch (c) {
                                case LBF_ES_UP:
                                case LBF_ES_DOWN:
                                case LBF_ES_LEFT:
                                case LBF_ES_RIGHT:
                                case LBF_ES_MOVE:
                                case LBF_ES_CLEAR:
                                case LBF_ES_GRAPHICS:
                                        escseq = false;
                                        count--;
                                break;
                        };
                        // Separate block allows reuse of sequence enders.
                        if (escseq) {
                                switch (c) {
                                        case LBF_ES_ALIGN_LEFT:
                                        case LBF_ES_ALIGN_CENTER:
                                        case LBF_ES_ALIGN_RIGHT:
                                                escseq = false;
                                                count--;
                                                break;
                                };
                        };
                };
                if (escseq == false) count++;
                i++;
                c = source[i];
        };
        *dest = count;
        return 0;
};

int string_count_size (int * dest, char * source) {
        if (source == NULL || dest == NULL) return -1;
        *dest = 0;
        while (source[*dest] != LBF_NULL) {
                (*dest)++;
        };
        (*dest)++;
        return 0;
};

bool string_is_equal (char * str_a, char * str_b) {
        if (str_a == NULL && str_b == NULL) return true;
        if (str_a == NULL || str_b == NULL) return false;
        int i = 0;
        char a;
        char b;
        do {
                a = str_a[i];
                b = str_b[i];
                if (a != b) return false;
                i++;
        } while (a != LBF_NULL && b != LBF_NULL);
        return true;
};

int string_del (char * str, int * index, int * count) {
        if (*count == 0) return 0;
        if (*index < 0) return -1;
        int counter = 0;
        int size;
        if (string_count_size(&size, str) != 0) return -1;
        char c;
        do {
                c = str[(*index) + (*count) + counter];
                str[(*index) + counter] = c;
                counter++;
        } while (c != LBF_NULL);
        return 0;
};

int string_ins_char (char * dest, int * index, char * source) {
        int i;
        if (string_count_size(&i, dest) != 0) return -1;
        do {
                dest[i+1] = dest[i];
                i--;
        } while (i >= *index);
        dest[*index] = *source;
        return 0;
};

int string_ins_string (char * dest, int * d_idx, char * source, int * s_idx,
                       int * count) {
        int di, i = 0;
        char sc;
        while (i < *count) {
                di = (*d_idx) + i;
                sc = source[(*s_idx) + i];
                if (string_ins_char(dest, &di, &sc) != 0) return -1;
                i++;
        };
        return 0;
};

int string_copy (char * dest, int * d_idx, char * source, int * s_idx,
                 int * count) {
        int i;
        for (i = 0; i < *count; i++) {
                dest[*d_idx + i] = source[*s_idx + i];
        };
        dest[*d_idx + i] = LBF_NULL;
        return 0;
};

int string_cat (char * dest, char * source) {
        int size;
        if (string_count_size(&size, source) != 0) return -1;
        if (size == 1) return 0;
        int index;
        if (string_count_size(&index, dest) != 0) return -1;
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
        if (string_count_size(&count, string) != 0) return -1;
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
        if (string_count_size(&base, LBF_NUMBERS) != 0) return -1;
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
        if (string_count_size(&i, source) != 0) return -1;
        i -= 2;
        int ival;
        bool isneg = false;
        int exp = 1;
        while (i >= 0) {
                if (i == 0 && source[i] == LBF_MINUS) {
                        isneg = true;
                } else if (int_from_char(&ival, &(source[i])) != 0) {
                        return -1;
                } else {
                        total += ival * exp;
                };
                exp *= LBF_NUMBASE;
                i--;
        };
        if (isneg) total = -total;
        *dest = total;
        return 0;
};

int string_from_int (char * dest, int * source) {
        if (dest == NULL || source == NULL) goto exiterr;
        int n = *source;
        if (*source == 0) {
                if (string_cat(dest, "0") != 0) goto exiterr;
                return 0;
        };
        char * tmp = calloc(1, sizeof(char));
        if (tmp == NULL) goto exiterr;
        int size = LBF_NUMSEQ_LEN;
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
                        if (tmp == NULL) goto cleantmp;
                };
                tmp[ti] = LBF_NUMBERS[n % LBF_NUMBASE];
                ti++;
                n = n/10;
        };
        if (isneg) tmp[ti] = LBF_MINUS;
        tmp[ti+1] = LBF_NULL;
        if (string_reverse(tmp) != 0) goto cleantmp;
        if (string_cat(dest, tmp) != 0) goto cleantmp;
        free(tmp);
        return 0;
        cleantmp:
                free(tmp);
        exiterr:
        return -1;
};

int string_move_cur (char * dest, int * x, int * y) {
        char * rowstr = calloc(LBF_NUMSEQ_LEN, sizeof(char));
        if (rowstr == NULL) goto exiterr;
        if (string_from_int(rowstr, y) != 0) goto cleanrow;
        char * colstr = calloc(LBF_NUMSEQ_LEN, sizeof(char));
        if (colstr == NULL) goto cleanrow;
        if (string_from_int(colstr, x) != 0) goto cleancol;
        char * seq = calloc(LBF_ESCSEQ_LEN, sizeof(char));
        if (seq == NULL) goto cleancol;
        char tmp = LBF_ESCAPE;
        int index = 0;
        if (string_ins_char(seq, &index, &tmp) != 0) goto cleanseq;
        tmp = LBF_OSB;
        index++;
        if (string_ins_char(seq, &index, &tmp) != 0) goto cleanseq;
        if (string_cat(seq, rowstr) != 0) goto cleanseq;
        if (string_cat(seq, ";") != 0) goto cleanseq;
        if (string_cat(seq, colstr) != 0) goto cleanseq;
        tmp = LBF_ES_MOVE;
        if (string_cat(seq, &tmp) != 0) goto cleanseq;
        if (string_cat(dest, seq) != 0) goto cleanseq;
        free(rowstr);
        free(colstr);
        free(seq);
        return 0;
        cleanseq:
                free(seq);
        cleancol:
                free(colstr);
        cleanrow:
                free(rowstr);
        exiterr:
        return -1;
};
