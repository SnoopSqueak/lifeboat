#ifndef CONLIST_H
#define CONLIST_H

#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct con {
        ssize_t id;
        ssize_t sockfd;
        struct sockaddr_in addr;
};

struct conlist {
        size_t length;
        struct con * head;
        size_t next_con_id;
        struct logger * dest_logger;
};

struct conlist * make_conlist (size_t length);
int free_conlist (struct conlist * clist);

int close_con (struct con * c, struct logger * dest_logger);
int make_con (in_addr_t address, uint16_t port, struct conlist * clist);
int log_con (struct logger * dest_logger, struct con * c, int msg_log_lvl);

#endif /* CONLIST_H */
