#include "lb_string.h"

struct lb_string * init_ntstring (char * ntstring) {
        struct lb_string * str = malloc(sizeof(struct lb_string));
        size_t count = 1;
        while (ntstring[count - 1] != '\0') count++;
        str->ntsize = count;
        str->ntstring = malloc(count * sizeof(char));
        strcpy(str->ntstring, ntstring);
        return str;
};

struct lb_string * init_utstring (char * utstring, size_t nchar) {
        struct lb_string * str = malloc(sizeof(struct lb_string));
        str->ntsize = nchar + 1;
        str->ntstring = malloc(str->ntsize * sizeof(char));
        size_t count = 0;
        while (count < nchar) {
                str->ntstring[count] = utstring[count];
                count++;
        };
        str->ntstring[count] = '\0';
        return str;
};

int grow_string (struct lb_string * str, size_t new_size) {
        if (new_size < 0) {
                errno = ERANGE;
                return -1;
        };
        str->ntstring = realloc(str->ntstring, new_size);
        size_t old_i = str->ntsize - 1;
        while (old_i < new_size - 1) {
                str->ntstring[old_i] = ' ';
                old_i++;
        };
        str->ntstring[old_i] = '\0';
        str->ntsize = new_size;
        return 0;
};

int put_in_string (struct lb_string * dest, size_t index, struct lb_string * src) {
        size_t siz = src->ntsize;
        if (siz == 0) return 0;
        grow_string(dest, dest->ntsize + src->ntsize - 1);
        size_t i = dest->ntsize - 1;
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

int take_from_string (struct lb_string * dest, size_t index, ssize_t count) {
        if (index >= dest->ntsize - 1 || count >= dest->ntsize) {
                errno = ERANGE;
                return -1;
        };
        if (count == -1) count = dest->ntsize - index;
        size_t c = 0;
        while (c < dest->ntsize) {
                dest->ntstring[index + c] = dest->ntstring[index + c + count];
                c++;
        };
        dest->ntstring = realloc(dest->ntstring, dest->ntsize - count);
        dest->ntsize = dest->ntsize - count;
        return 0;
};

int string_to_int (struct lb_string * str) {
        return 0;
};

struct lb_string * int_to_string (int n) {
        return init_ntstring("NaN");
};

int free_string (struct lb_string * str) {
        free(str->ntstring);
        free(str);
        return 0;
};
