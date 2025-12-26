#ifndef LB_THREADS_H
#define LB_THREADS_H

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

struct lb_thread;
struct lbt_item {
        int lbtid;
        struct lb_thread * value;
        struct lbt_item * next;
};

int init_lbthreads ();
int lb_fork (int * chid, int (* chld_func) (void));
int lb_raise (int lbtid, int sig);

#endif /* LB_THREADS_H */
