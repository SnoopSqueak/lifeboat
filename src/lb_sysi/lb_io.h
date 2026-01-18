#ifndef LB_IO_H
#define LB_IO_H

#include <errno.h>
#include <linux/kd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#include "lb_bool.h"
#include "lb_fmt.h"
#include "lb_str.h"

struct lb_file;
struct lb_tty_cfg;
struct lb_kbevent {
        int code;
};

int init_file_stdin (struct lb_file **dest);
int init_file_stdout (struct lb_file **dest);
int file_put_clear (struct lb_file *dest);
int file_put_str (struct lb_file *dest, const struct lb_str *src);
int file_get_key (struct lb_kbevent *dest, const struct lb_file *src);
int free_file (struct lb_file **dest);
int init_tty_cfg (struct lb_tty_cfg **dest);
int free_tty_cfg (struct lb_tty_cfg **dest);

#endif /* LB_IO_H */
