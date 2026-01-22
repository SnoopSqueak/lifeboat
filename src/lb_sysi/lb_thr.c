#include <stdatomic.h>
#include <threads.h>

#include "lb_thr.h"

struct lb_thri *lbthead = NULL;
unsigned int lbtid = 0;
mtx_t lbtmtx;
int is_lbt_init = LB_FALSE;

struct lb_thr {
        thrd_t *thr;
};

struct lb_thri {
        unsigned int lbtid;
        unsigned int lbtpid;
        struct lb_thr *value;
        struct lb_thri *next;
};

int init_thr (unsigned int *dest, const int (*func) (void)) {
        if (mtx_lock(&lbtmtx) != thrd_success) return -1;
        if (lbtid == 0 && func != NULL) return -1;
        if (lbtid > 0 && func == NULL) return -1;
        struct lb_thri *lbti = malloc(sizeof(struct lb_thri));
        lbti->lbtid = lbtid++;
        lbti->next = NULL;
        lbti->value = malloc(sizeof(struct lb_thr));
        lbti->value->thr = malloc(sizeof(thrd_t));
        if (func != NULL && thrd_create(lbti->value->thr,
                (thrd_start_t) (*func), NULL) != thrd_success)
                goto cleanup;
        struct lb_thri *li = lbthead;
        if (li == NULL) {
                lbthead = lbti;
        } else {
                while (li->next != NULL) {
                        li = li->next;
                };
                li->next = lbti;
        };
        if (mtx_unlock(&lbtmtx) != thrd_success) return -1;
        return 0;
cleanup:
        free(li->value->thr);
        free(li->value);
        free(li);
        return -1;
};

int init_lbthreads () {
        if (is_lbt_init == LB_TRUE) return -1;
        if (mtx_init(&lbtmtx, mtx_plain) != thrd_success) return -1;
        unsigned int id;
        if (init_thr(&id, NULL) != 0) goto cleanmtx;
        is_lbt_init = LB_TRUE;
        return 0;
cleanmtx:
        mtx_destroy(&lbtmtx);
        return -1;
};

int lb_raise (int *lbtid, int *sig) {
        return -1;
};

int free_thr (const int *lbtid) {
        if (mtx_lock(&lbtmtx) != thrd_success) return -1;
        struct lb_thri *li = lbthead;
        struct lb_thri *pli;
        while (li->next != NULL && li->lbtid != *lbtid) {
                pli = li;
                li = li->next;
        };
        pli->next = li->next;
        if (mtx_unlock(&lbtmtx) != thrd_success) return -1;
        // todo? raise exit signal for thread
        free(li->value->thr);
        free(li->value);
        free(li);
        return 0;
};
