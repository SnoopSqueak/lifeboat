#include "lb_threads.h"

#include <stdatomic.h>
#include <threads.h>

struct lbt_item * lbt_head;
tss_t tss_lbtid;
atomic_int next_lbtid;
mtx_t * lbt_mtx;

struct lb_thread {
        thrd_t * thr;
};

int init_thread (int * lbtid, int (* entry_func) (void)) {
        struct lbt_item * lbti = malloc(sizeof(struct lbt_item));
        lbti->lbtid = atomic_fetch_add(&next_lbtid, 1);
        *lbtid = lbti->lbtid;
        lbti->next = NULL;
        lbti->value = malloc(sizeof(struct lb_thread));
        lbti->value->thr = malloc(sizeof(thrd_t));
        struct lbt_item * li = lbt_head;
        if (li == NULL) {
                lbt_head = lbti;
        } else {
                while (li->next != NULL) {
                        li = li->next;
                };
                li->next = lbti;
                if (thrd_create(lbti->value->thr, (thrd_start_t) (* entry_func), NULL) != thrd_success) return -1;
        };
                if (tss_set(tss_lbtid, &lbtid) != thrd_success) return -1;
                return 0;
};

int init_lbthreads () {
        if (tss_create(&tss_lbtid, free) != thrd_success) return -1;
        atomic_init(&next_lbtid, 0);
        lbt_mtx = malloc(sizeof(mtx_t));
        if (mtx_init(lbt_mtx, mtx_plain) != 0) return -1;
        if (mtx_lock(lbt_mtx) != thrd_success) return -1;
        lbt_head = NULL;
        int id = 0;
        if (init_thread(&id, NULL) != 0) return -1;
        if (mtx_unlock(lbt_mtx) != thrd_success) return -1;
        return 0;
};

int lb_fork (int * chid, int (* entry_func) (void)) {
        if (mtx_lock(lbt_mtx) != thrd_success) return -1;
        if (init_thread(chid, entry_func) != 0) return -1;
        if (mtx_unlock(lbt_mtx) != thrd_success) return -1;
        return 0;
};

int lb_raise (int lbtid, int sig) {
        return -1;
};

int free_thread (int lbtid) {
        struct lbt_item * li = lbt_head;
        struct lbt_item * pli;
        while (li->next != NULL && li->lbtid != lbtid) {
                pli = li;
                li = li->next;
        };
        pli->next = li->next;
        free(li->value->thr);
        free(li->value);
        free(li);
        return 0;
};
