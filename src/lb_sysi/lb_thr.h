#ifndef LB_THR_H
#define LB_THR_H

#include <stdlib.h>
#include <time.h>

#include "lb_bool.h"

int init_lbthreads ();
int init_thr (unsigned int *dest, const int (*func) (void));
int thr_sleep (const unsigned int *ms);
// int lb_raise (int *lbtid, int *sig);
int free_thr (const int *lbtid);

#endif /* LB_THR_H */
