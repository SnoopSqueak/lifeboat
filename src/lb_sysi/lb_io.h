#ifndef LB_IO_H
#define LB_IO_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

struct lb_file;
struct lb_tty_cfg;
struct lb_kbevent {
        int code;
        bool is_press;
};

int file_init_tty_in (struct lb_file **dest);
int file_init_tty_out (struct lb_file **dest);
int file_put_string (struct lb_file *dest, char *str);
int file_get_key (struct lb_kbevent *dest, struct lb_file *source);
int file_free (struct lb_file **dest);
int tty_cfg_init (struct lb_tty_cfg **dest);
int tty_cfg_free (struct lb_tty_cfg **dest);

#endif /* LB_IO_H */
