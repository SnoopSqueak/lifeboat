#include "conlist.h"
#include "logger.h"

struct conlist * make_conlist (size_t length) {
        struct conlist * clist = malloc(sizeof(struct conlist));
        clist->head = calloc(length, sizeof(struct con));
        clist->next_con_id = 0;
        for (size_t i = 0; i < length; i++) {
                clist->head[i].id = -1;
                clist->head[i].addr = (struct sockaddr_in) {AF_INET, 0, {0}};
                clist->head[i].sockfd = -1;
        }
        return clist;
};

//~ con * get_any_con (ssize_t con_id) {
	//~ con * c = get_con_by_id(con_id, &host_connections);
	//~ if (con != NULL) {
		//~ log_this("Found connection %i in host connections list.\n", DEBUG, con_id);
	//~ } else {
		//~ con = get_con_by_id(con_id, &client_connections);
		//~ if (con != NULL) {
			//~ log_this("Found connection %i in client connections list.\n", DEBUG, con_id);
		//~ }
	//~ }
	//~ if (con == NULL) {
		//~ log_this("Failed to find connection with ID %i.\n", ERROR, con_id);
	//~ }
	//~ return con;
//~ }

int free_conlist (struct conlist * clist) {
        free(clist->head);
        free(clist);
        return 0;
};

int close_con (struct con * c) {
        if (c == NULL) return 0;
        // close socket
        if (c->sockfd != -1) {
                shutdown(c->sockfd, SHUT_RDWR);
                close(c->sockfd);
        }
        // clear connection
        c->addr.sin_family = AF_INET;
        c->addr.sin_port = 0;
        c->addr.sin_addr.s_addr = 0;
        c->sockfd = -1;
        c->id = -1;
        return 0;
}

int make_con (in_addr_t addr, uint16_t port, struct conlist * clist) {
        // make socket
        ssize_t sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) return -1;
        // find available connection slot
        size_t i = 0;
        size_t length = clist->length;
        struct con * c;
        ssize_t first_free = -1;
        while (i < length) {
                c = &(clist->head[i]);
                if (first_free == -1 && c->id == -1) first_free = i;
                if (c->sockfd == sockfd) return -1;
                if (c->addr.sin_port == port && c->addr.sin_addr.s_addr == addr) return -1;
                i++;
        }
        if (first_free >= length) return -1;
        // bind socket
        if (bind(sockfd, (struct sockaddr *) &(c->addr), sizeof(struct sockaddr)) == -1) {
                close_con(c);
                return -1;
        }
        // listen on socket
        if (listen(sockfd, clist->length) == -1) {
                close_con(c);
                return -1;
        }
        // store connection data
        c = &(clist->head[first_free]);
        c->addr.sin_family = AF_INET;
        c->addr.sin_port = htons(port);
        c->addr.sin_addr.s_addr = htonl(addr);
        c->sockfd = sockfd;
        c->id = clist->next_con_id;
        clist->next_con_id++;
        return 0;
}
