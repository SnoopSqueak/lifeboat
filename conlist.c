#include "conlist.h"
#include "logger.h"

struct conlist * make_conlist (size_t length) {
        struct conlist * clist = malloc(sizeof(struct conlist));
        clist->head = calloc(length, sizeof(struct con));
        clist->next_con_id = 0;
        clist->dest_logger = make_logger(LL_NEVER, stdout, CF_ANSI);
        for (size_t i = 0; i < length; i++) {
                clist->head[i].id = -1;
                clist->head[i].addr = (struct sockaddr_in) {AF_INET, 0, {0}};
                clist->head[i].sockfd = -1;
        }
        return clist;
};

int free_conlist (struct conlist * clist) {
        free(clist->head);
        free(clist);
        return 0;
};

int close_con (struct con * c, struct logger * dest_logger) {
        if (c == NULL) {
                log_this(dest_logger, "Tried to close null connection.\n", LL_WARN);
                return 0;
        }
        
        // close socket
        if (c->sockfd != -1) {
                shutdown(c->sockfd, SHUT_RDWR);
                close(c->sockfd);
                log_this(dest_logger, "Closed socket %u.\n", LL_DEBUG, c->sockfd);
        }
        
        // clear connection
        ssize_t old_id = c->id;
        c->addr.sin_family = AF_INET;
        c->addr.sin_port = 0;
        c->addr.sin_addr.s_addr = 0;
        c->sockfd = -1;
        c->id = -1;
        log_this(dest_logger, "Cleared connection formerly known as #%i.\n", LL_DEBUG, old_id);
        return 0;
}

int make_con (in_addr_t addr, uint16_t port, struct conlist * clist) {
        // make socket
        log_this(clist->dest_logger, "Opening socket at %u:%u...\n", LL_DEBUG, addr, port);
        ssize_t sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
                log_this(clist->dest_logger, "Failed to open the socket: %s\n", LL_WARN, strerror(errno));
                return -1;
        }
        
        // find available connection slot
        size_t i = 0;
        size_t length = clist->length;
        struct con * c;
        ssize_t first_free = -1;
        while (i < length) {
                c = &(clist->head[i]);
                if (first_free == -1 && c->id == -1) {
                        first_free = i;
                }
                if (c->sockfd == sockfd) {
                        log_this(clist->dest_logger, "Given socket %i already exists as a known connection.\n", LL_WARN, sockfd);
                        return -1;
                }
                if (c->addr.sin_port == port && c->addr.sin_addr.s_addr == addr) {
                        log_this(clist->dest_logger, "Socket %i is already open at %i port %i.\n", LL_WARN, sockfd, addr, port);
                        return -1;
                }
                i++;
        }
        if (first_free >= length) {
                log_this(clist->dest_logger, "Failed to make new connection, list is full.\n", LL_ERROR);
                return -1;
        }
        
        // bind socket
        if (bind(sockfd, (struct sockaddr *) &(c->addr), sizeof(struct sockaddr)) == -1) {
                log_this(clist->dest_logger, "Failed to bind the socket: %s\n", LL_WARN, strerror(errno));
                close_con(c, clist->dest_logger);
                return 0;
        }
        log_this(clist->dest_logger, "Bound socket %i to port %i.\n", LL_DEBUG, sockfd, port);
        
        // listen on socket
        if (listen(sockfd, clist->length) == -1) {
                log_this(clist->dest_logger, "Failed to listen for connections: %s\n", LL_WARN, strerror(errno));
                close_con(c, clist->dest_logger);
                return 0;
        }
        log_this(clist->dest_logger, "Listening for connections on port %i.\n", LL_INFO, port);
        
        // store connection data
        c = &(clist->head[first_free]);
        c->addr.sin_family = AF_INET;
        c->addr.sin_port = htons(port);
        c->addr.sin_addr.s_addr = htonl(addr);
        c->sockfd = sockfd;
        c->id = clist->next_con_id;
        clist->next_con_id++;
        log_this(clist->dest_logger, "Created connection %u at index %u.\n", LL_DEBUG, c->id, first_free);
        return 0;
}

int log_con (struct logger * dest_logger, struct con * c, int msg_log_lvl) {
        char * output = "id: %i. sockfd: %i. address: %u. port: %u.\n";
        return log_this(dest_logger, output, msg_log_lvl, c->id,
        c->sockfd, c->addr.sin_addr.s_addr, ntohs(c->addr.sin_port));
}
