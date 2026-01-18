#include "lb_str.h"

static const int AS_FIRSTNUM = 48;

int count_chars (int *dest, const char *src) {
        if (src == NULL || dest == NULL) return -1;
        *dest = 0;
        while (src[*dest] != LBF_NULL) {
                (*dest)++;
                if (*dest >= LBF_MAX_STR_LEN - 1) {
                        errno = ERANGE;
                        return -1;
                };
        };
        (*dest)++;
        return 0;
};

int chars_ins_chars (char *dest, const int *di, const char *src, const int *si,
                     const int *count) {
        int dsize, i = 0;
        if (dest == NULL) return -1;
        if (count_chars(&dsize, dest) != 0) return -1;
        if (dsize < 1) return -1;
        while ((*di) + i + (*count) <= dsize) {
                dest[(*di) + i + (*count)] = dest[(*di) + i];
                i++;
        };
        i = 0;
        while (i < *count) {
                dest[(*di) + i] = src[(*si) + i];
                i++;
        };
        return 0;
};

int int_from_char (int *dest, const char *src) {
        int base;
        if (count_chars(&base, LBF_NUMBERS) != 0) return -1;
        if (*src < AS_FIRSTNUM || *src > AS_FIRSTNUM + base) {
                errno = ERANGE;
                return -1;
        };
        *dest = *src - AS_FIRSTNUM;
        return 0;
};

int int_from_chars (int *dest, const char *src) {
        int i, total = 0, ival, isneg = LB_FALSE, exp = 1;
        if (count_chars(&i, src) != 0) return -1;
        i -= 2;
        while (i >= 0) {
                if (i == 0 && src[i] == LBF_MINUS) {
                        isneg = LB_TRUE;
                } else {
                        if (int_from_char(&ival, &(src[i])) != 0) return -1;
                        total += ival * exp;
                };
                exp *= LBF_NUMBASE;
                i--;
        };
        if (isneg == LB_TRUE) total = -total;
        *dest = total;
        return 0;
};

int valid_str (const struct lb_str *str) {
        if (str == NULL || str->chars == NULL) return LB_FALSE;
        return LB_TRUE;
};

int init_str_str (struct lb_str **dest, const struct lb_str *src) {
        if (!valid_str(src)) return -1;
        int ssize;
        if (count_str_chars(&ssize, src) != 0) return -1;
        if (init_str_charcount(dest, &ssize) != 0) return -1;
        (*dest)->chars = malloc(sizeof(src->chars));
        if (!valid_str(*dest)) {
                errno = ENOMEM;
                return -1;
        };
        if (str_cat_str(*dest, src) != 0) goto cleandest;
        return 0;
cleandest:
        free_str(dest);
        return -1;
};

int init_str_charcount (struct lb_str **dest, const int *charcount) {
        if (*charcount < 1 || *charcount > LBF_MAX_STR_LEN) {
                errno = ERANGE;
                return -1;
        };
        *dest = malloc(sizeof(struct lb_str));
        if (*dest == NULL) goto memerr;
        (*dest)->chars = malloc(*charcount * sizeof(char));
        if ((*dest)->chars == NULL) goto cleandest;
        (*dest)->size = (*charcount) * sizeof(char);
        (*dest)->chars[0] = LBF_NULL;
        if (!valid_str(*dest)) goto memerr;
        return 0;
cleandest:
        free(*dest);
        *dest = NULL;
memerr:
        errno = ENOMEM;
        return -1;
};

int str_ins_chars (struct lb_str *dest, const int *di, const char *src,
                   const int *si, const int *count) {
        if (!valid_str(dest)) return -1;
        int ssize, dsize, newsize;
        if (count_chars(&ssize, src) != 0) return -1;
        if (ssize == 1) return 0;
        if (count_str_chars(&dsize, dest) != 0) return -1;
        newsize = dsize + ssize - 1;
        if (dest->size < newsize) {
                while (dest->size < newsize) {
                        dest->size *= 2;
                        if (dest->size > LBF_MAX_STR_LEN) {
                                dest->size = LBF_MAX_STR_LEN;
                                errno = ERANGE;
                                return -1;
                        };
                };
                dest->chars = realloc(dest->chars, dest->size);
        };
        if (chars_ins_chars(dest->chars, di, src, si, count) != 0) return -1;
        return 0;
};

int init_str_chars (struct lb_str **dest, const char *src) {
        if (dest == NULL) return -1;
        const int i = 0;
        int count;
        if (count_chars(&count, src) != 0) return -1;
        if (init_str_charcount(dest, &count) != 0) return -1;
        if (str_ins_chars(*dest, &i, src, &i, &count) != 0) goto cleandest;
        return 0;
cleandest:
        free_str(dest);
        return -1;
};

int count_str_chars (int *dest, const struct lb_str *src) {
        if (!valid_str(src)) return -1;
        return count_chars(dest, src->chars);
};

int char_from_str (char *dest, const struct lb_str *src, const int *i) {
        if (dest == NULL || !valid_str(src) || i == NULL) return -1;
        int size;
        if (count_str_chars(&size, src) != 0) return -1;
        if (size <= *i) {
                errno = ERANGE;
                return -1;
        };
        *dest = src->chars[*i];
        return 0;
};

int comp_str_str (int *dest, const struct lb_str *src_a,
                  const struct lb_str *src_b) {
        if (!valid_str(src_a) || !valid_str(src_b)) return -1;
        int i = 0;
        char a, b;
        do {
                a = src_a->chars[i];
                b = src_b->chars[i];
                if (a != b) {
                        *dest = 0;
                        return 0;
                };
                i++;
        } while (a != LBF_NULL && b != LBF_NULL);
        *dest = 1;
        return 0;
};

int str_ins_str (struct lb_str *dest, const int *di, const struct lb_str *src,
                 const int *si, const int *count) {
        if (!valid_str(dest) || !valid_str(src)) return -1;
        if (src->chars[0] == '\0') return 0;
        return str_ins_chars(dest, di, src->chars, si, count);
};

int str_cat_char (struct lb_str *dest, const char *src) {
        const int si = 0, amt = 1;
        int di;
        if (count_str_chars(&di, dest) != 0) return -1;
        di--;
        return str_ins_chars(dest, &di, src, &si, &amt);
};

int str_cat_chars (struct lb_str *dest, const char *src) {
        int si = 0, di, scount;
        if (count_str_chars(&di, dest) != 0) return -1;
        di--;
        if (count_chars(&scount, src) != 0) return -1;
        if (str_ins_chars(dest, &di, src, &si, &scount) != 0) return -1;
        return 0;
};

int str_cat_str (struct lb_str *dest, const struct lb_str *src) {
        int si = 0, di, scount;
        if (count_str_chars(&di, dest) != 0) return -1;
        di--;
        if (count_str_chars(&scount, src) != 0) return -1;
        if (str_ins_str(dest, &di, src, &si, &scount) != 0) return -1;
        return 0;
};

int str_reverse (struct lb_str *str) {
        char swp;
        int i = 0, count;
        if (count_str_chars(&count, str) != 0) return -1;
        count--;
        while (i < count / 2) {
                swp = str->chars[i];
                str->chars[i] = str->chars[count - 1 - i];
                str->chars[count - 1 - i] = swp;
                i++;
        };
        return 0;
};

int init_str_int (struct lb_str **dest, const int *src) {
        if (src == NULL) return -1;
        if (*src == 0) {
                if (init_str_chars(dest, "0") != 0) return -1;
                return 0;
        };
        int size = LBF_NUMSEQ_LEN, n = *src, i = 0, si;
        const int amt = 1, isneg = n < 0 ? LB_TRUE : LB_FALSE;
        const char *nstr = LBF_NUMBERS, negch = LBF_MINUS, nulch = LBF_NULL;
        if (isneg) {
                size++;
                n *= -1;
        };
        if (init_str_charcount(dest, &size) != 0) return -1;
        if (!valid_str(*dest)) return -1;
        while (n != 0) {
                si = n % LBF_NUMBASE;
                if (str_ins_chars(*dest, &i, nstr, &si, &amt) != 0)
                        goto cleandest;
                i++;
                n = n/10;
        };
        if (isneg) {
                if (str_cat_char(*dest, &negch) != 0) goto cleandest;
        };
        if (str_reverse(*dest) != 0) goto cleandest;
        return 0;
cleandest:
        free_str(dest);
        return -1;
};

int str_del_charcount (struct lb_str *str, const int *i, const int *count) {
        if (*count == 0) return 0;
        if (*i < 0 || !valid_str(str)) return -1;
        int counter = 0, size;
        if (count_str_chars(&size, str) != 0) return -1;
        if (*i + *count >= size) {
                errno = ERANGE;
                return -1;
        };
        char c;
        do {
                c = str->chars[(*i) + (*count) + counter];
                str->chars[(*i) + counter] = c;
                counter++;
        } while (c != LBF_NULL);
        return 0;
};

int str_del_all (struct lb_str *str) {
        if (!valid_str(str)) return -1;
        str->chars[0] = LBF_NULL;
        return 0;
};

int free_str (struct lb_str **dest) {
        if (dest == NULL) return -1;
        if (*dest != NULL) {
                free((*dest)->chars);
                free(*dest);
                *dest = NULL;
        };
        return 0;
};
