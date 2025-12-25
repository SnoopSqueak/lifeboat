#include "lb_string.h"

struct lb_string * init_ntstring (char * ntstring) {
        struct lb_string * str = malloc(sizeof(struct lb_string));
        if (str == NULL) return NULL;
        int count = 0;
        while (ntstring[count] != '\0') count++;
        count++;
        str->ntsize = count;
        str->ntstring = malloc(count * sizeof(char));
        if (str->ntstring == NULL) return NULL;
        str->maxsize = -1;
        count = 0;
        while (count < str->ntsize) {
                str->ntstring[count] = ntstring[count];
                count++;
        }
        return str;
};

struct lb_string * init_utstring (char * utstring, int nchar) {
        struct lb_string * str = malloc(sizeof(struct lb_string));
        if (str == NULL) return NULL;
        str->ntsize = nchar + 1;
        str->ntstring = malloc(str->ntsize * sizeof(char));
        if (str->ntstring == NULL) return NULL;
        str->maxsize = -1;
        int count = 0;
        while (count < nchar) {
                str->ntstring[count] = utstring[count];
                count++;
        };
        str->ntstring[count] = '\0';
        return str;
};

int count_ntstring (char * ntstring) {
        if (ntstring == NULL) return -1;
        int i = 0;
        while (ntstring[i] != '\0') i++;
        return i;
};

int grow_string (struct lb_string * string, int new_size) {
        if (string->maxsize > -1 && new_size > string->maxsize) {
                errno = ERANGE;
                return -1;
        };
        if (new_size < string->ntsize) {
                errno = ERANGE;
                return -1;
        };
        int old_i = string->ntsize - 1;
        string->ntstring = realloc(string->ntstring, new_size);
        if (string->ntstring == NULL) {
                errno = ENOMEM;
                return -1;
        };
        while (old_i < new_size - 1) {
                string->ntstring[old_i] = ' ';
                old_i++;
        };
        string->ntstring[old_i] = '\0';
        string->ntsize = new_size;
        return 0;
};

int shrink_string (struct lb_string * string, int newsize) {
        if (newsize == string->ntsize) return 0;
        if (newsize < 1 || newsize > string->ntsize) {
                errno = ERANGE;
                return -1;
        };
        string->ntstring = realloc(string->ntstring, newsize);
        if (string->ntstring == NULL) {
                errno = ENOMEM;
                return -1;
        };
        string->ntsize = newsize;
        string->ntstring[newsize - 1] = '\0';
        return 0;
};

int put_in_string (struct lb_string * dest, int index, struct lb_string * src) {
        int siz = src->ntsize;
        if (siz == 0) return 0;
        if (index == -1) index = dest->ntsize - 1;
        if (grow_string(dest, dest->ntsize + src->ntsize - 1) == -1) return -1;
        int i = dest->ntsize - 1;
        while (i > index) {
                dest->ntstring[i - 1] = dest->ntstring[i - siz];
                i--;
        };
        i = 0;
        while (src->ntstring[i] != '\0') {
                dest->ntstring[index] = src->ntstring[i];
                i++;
                index++;
        };
        return 0;
};

int take_from_string (struct lb_string * dest, int index, int count) {
        if (count == -1) count = dest->ntsize - index - 1;
        if (index > dest->ntsize - 1 || count > dest->ntsize - 1 - index || index < 0) {
                errno = ERANGE;
                return -1;
        };
        if (count == 0) return 0;
        int c = 0;
        while (c < dest->ntsize - 1) {
                if (index + c + count < dest->ntsize) {
                        dest->ntstring[index + c] = dest->ntstring[index + c + count];
                };
                c++;
        };
        if (shrink_string(dest, dest->ntsize - count) == -1) return -1;
        return 0;
};

int clear_string (struct lb_string * string) {
        return take_from_string(string, 0, -1);
};

int reverse_string (struct lb_string * str) {
        char swp;
        int i = 0;
        while (i < str->ntsize / 2) {
                swp = str->ntstring[i];
                str->ntstring[i] = str->ntstring[str->ntsize - 2 - i];
                str->ntstring[str->ntsize - 2 - i] = swp;
                i++;
        }
        return 0;
};

int char_to_int (char c, int * dest) {
        int base = count_ntstring(ONESLIST);
        if (base == -1) return -1;
        if (c < AS_FIRSTNUM || c > AS_FIRSTNUM + base) {
                errno = ERANGE;
                return -1;
        };
        *dest = c - AS_FIRSTNUM;
        return 0;
};

bool is_minus_char (char c) {
        return (c == MINUSSTRING[0]);
};

int string_to_int (struct lb_string * str, int * dest) {
        int total = 0;
        int i = str->ntsize - 2;
        int * numptr = malloc(sizeof(int));
        bool isneg = false;
        int exp = 1;
        while (i >= 0) {
                if (i == 0 && is_minus_char(str->ntstring[i])) {
                        isneg = true;
                } else if (char_to_int(str->ntstring[i], numptr) == -1) {
                        free(numptr);
                        return -1;
                } else {
                        total += *numptr * exp;
                };
                exp *= NUMBASE;
                i--;
        };
        if (isneg) total = -total;
        *dest = total;
        free(numptr);
        return 0;
};

struct lb_string * int_to_string (int n) {
        struct lb_string * result = init_ntstring("");
        if (result == NULL) return NULL;
        struct lb_string * tmp;
        bool isneg = n < 0;
        while (n != 0) {
                tmp = init_utstring(&ONESLIST[n % NUMBASE], 1);
                if (tmp == NULL) return NULL;
                if (put_in_string(result, 0, tmp) == -1) {
                        free_string(&tmp);
                        return NULL;
                };
                free_string(&tmp);
                n = n/10;
        };
        if (isneg) {
                tmp = init_ntstring(MINUSSTRING);
                if (tmp == NULL) return NULL;
                if (put_in_string(result, 0, tmp) == -1) {
                        free_string(&tmp);
                        return NULL;
                };
                free_string(&tmp);
        };
        return result;
};

int free_string (struct lb_string ** str) {
        free((*str)->ntstring);
        free(*str);
        *str = NULL;
        return 0;
};
