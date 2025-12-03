// Mike Stowe 2025
// https://snoopsqueak.com

#include <errno.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 2048
#define MIN_INPUT_LENGTH 2
#define MAX_PARAMS_LENGTH 32
#define MAX_INCOMING_CONNECTIONS 64
//#define MAX_OUTGOING_CONNECTIONS 64
#define MAX_OUTGOING_CONNECTIONS 4
#define SOFTWARE_NAME "LifeBoat"

#define DEFAULT_PORT 25252

#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_BRIGHT_BLACK "\x1b[90m"
#define ANSI_COLOR_BRIGHT_RED "\x1b[91m"
#define ANSI_COLOR_BRIGHT_GREEN "\x1b[92m"
#define ANSI_COLOR_BRIGHT_YELLOW "\x1b[93m"
#define ANSI_COLOR_BRIGHT_BLUE "\x1b[94m"
#define ANSI_COLOR_BRIGHT_MAGENTA "\x1b[95m"
#define ANSI_COLOR_BRIGHT_CYAN "\x1b[96m"
#define ANSI_COLOR_BRIGHT_WHITE "\x1b[97m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define NEVER 0
#define TRACE 1
#define DEBUG 2
#define INFO 3
#define WARN 4
#define ERROR 5
#define ALWAYS 6

int log_level = NEVER;

size_t next_con_id = 0;

typedef struct {
	char * command;
	char * about;
	int (* handler)(char ** argv);
} Command;

typedef struct Connection {
	struct sockaddr_in addr;
	ssize_t socket_fd;
	ssize_t id;
} Connection;

typedef struct ConnectionList {
	size_t length;
	Connection * head;
} ConnectionList;

ConnectionList host_connections = {MAX_OUTGOING_CONNECTIONS, nullptr};
//Connection *client_connections[MAX_INCOMING_CONNECTIONS];

void log_this (char * message, int message_log_level, ...) {
	if (message_log_level > NEVER && log_level <= message_log_level) {
		va_list args;
		va_start(args, message_log_level);
		char * color;
		switch (message_log_level) {
			case ALWAYS: color = ANSI_COLOR_BRIGHT_WHITE; break;
			case TRACE: color = ANSI_COLOR_BRIGHT_GREEN; break;
			case DEBUG: color = ANSI_COLOR_BRIGHT_BLACK; break;
			case INFO: color = ANSI_COLOR_BRIGHT_WHITE; break;
			case WARN: color = ANSI_COLOR_BRIGHT_YELLOW; break;
			case ERROR: color = ANSI_COLOR_BRIGHT_RED; break;
		}
		printf(color);
		vprintf(message, args);
		printf(ANSI_COLOR_RESET);
		va_end(args);
	}
}

int handle_quit (char ** argv) {
	log_this("Exiting program.\n", DEBUG);
	return 1;
}

int handle_help(char ** argv);

int handle_join(char ** argv);

int handle_host(char ** argv);

int handle_end_host(char ** argv);

int handle_end_join(char ** argv);

int handle_close(char ** argv);

int make_connection(ssize_t socket_fd, in_addr_t address, uint16_t port, ConnectionList * connections_list);

Command commands[] = {
	{"/help", "List available commands.", handle_help},
	{"/quit", "Exit this program.", handle_quit},
	{"/host", "[port]  Open your " SOFTWARE_NAME " for others to join the lobby on port [port].", handle_host},
	{"/join", "[address]  Connect to another " SOFTWARE_NAME " that is hosting at [address].", handle_join},
	{"/close", "[connection_number]  Close all open connections.", handle_close},
	{"/say", "[connection_number]  Send a message through the given channel(s).", handle_close}
};

void log_connection (Connection * con, int message_log_level) {
	char * output = "id: %i. socket_fd: %i. address: %u. port: %u.\n";
	log_this(output, message_log_level, con->id, con->socket_fd, con->addr.sin_addr.s_addr, ntohs(con->addr.sin_port));
}

int make_connection (ssize_t socket_fd, in_addr_t address, uint16_t port, ConnectionList * connections_list) {
	size_t i = 0;
	size_t length = connections_list->length;
	Connection * existing;
	ssize_t first_free = -1;
	while (i < length) {
		existing = &(connections_list->head[i]);
		if (first_free == -1 && existing->id == -1) {
			first_free = i;
		}
		if (existing->socket_fd == socket_fd) {
			log_this("Given socket %i already exists as a known connection.\n", WARN, socket_fd);
			return -1;
		}
		if (existing->addr.sin_port == port && existing->addr.sin_addr.s_addr == address) {
			log_this("Socket %i is already open at %i port %i.\n", WARN, socket_fd, address, port);
			return -1;
		}
		i++;
	}
	if (first_free >= length) {
		log_this("Failed to make new connection, list is full.\n", ERROR);
		return -1;
	}
	existing = &(connections_list->head[first_free]);
	existing->addr.sin_family = AF_INET;
	existing->addr.sin_port = htons(port);
	existing->addr.sin_addr.s_addr = htonl(address);
	existing->socket_fd = socket_fd;
	existing->id = next_con_id;
	next_con_id = next_con_id + 1;
	log_this("Created connection %u at index %u.\n", DEBUG, existing->id, first_free);
	return first_free;
}

int close_socket (ssize_t sockfd) {
	// shuts down the socket
	shutdown(sockfd, SHUT_RDWR);
	close(sockfd);
	log_this("Closed socket %u.\n", DEBUG, sockfd);
	return 0;
}

int clear_connection (Connection * con) {
	ssize_t old_id = con->id;
	con->addr.sin_family = AF_INET;
	con->addr.sin_port = 0;
	con->addr.sin_addr.s_addr = 0;
	con->socket_fd = -1;
	con->id = -1;
	log_this("Cleared connection formerly known as #%i.\n", DEBUG, old_id);
	return 0;
}

size_t get_active_connections (ConnectionList * connections_list, Connection * results) {
	size_t i = 0;
	size_t length = connections_list->length;
	Connection * existing;
	size_t count = 0;
	while (i < length) {
		existing = &(connections_list->head[i]);
		log_connection(existing, TRACE);
		if (existing->id != -1) {
			results[count] = connections_list->head[i];
			count = count + 1;
		}
		i++;
	}
	return count;
}

int handle_help (char ** argv) {
	Connection * active_host_sockets = calloc(host_connections.length, sizeof(Connection));
	size_t con_count = get_active_connections(&host_connections, active_host_sockets);
	if (con_count > 0) {
		log_this("%u active host socket(s):\n  ", INFO, con_count);
		for (size_t con_i = 0; con_i < con_count; con_i++) {
			if (con_i > 0) {
				log_this(", ", INFO);
			}
			log_this("%i %u", INFO, active_host_sockets[con_i].id, ntohs(active_host_sockets[con_i].addr.sin_port));
		}
		log_this(".\n", INFO);
	}
	free(active_host_sockets);
	log_this("Available commands:\n", INFO);
	int commands_length = sizeof(commands)/sizeof(commands[0]);
	for (int i = 0; i < commands_length; i++) {
		log_this("  %s  %s\n", INFO, commands[i].command, commands[i].about);
	}
	return 0;
}

int handle_join (char ** argv) {
	char * address = argv[0];
	log_this("Attempting to connect to %s...\n", DEBUG, address);
	log_this("Join feature has not yet been implemented.\n", WARN);
	return 0;
}

int handle_host (char ** argv) {
	uint16_t outgoing_port = DEFAULT_PORT;
	if (argv != NULL) {
		char * port_string = argv[0];
		if (port_string != NULL && strlen(port_string) > 0) {
			outgoing_port = strtoul(port_string, NULL, 10);
		}
	}
	log_this("Opening INADDR_ANY on port %u...\n", DEBUG, outgoing_port);
	ssize_t socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		log_this("Failed to open the socket: %s\n", WARN, strerror(errno));
		return 0;
	}
	ssize_t con_i = make_connection(socket_fd, INADDR_ANY, outgoing_port, &host_connections);
	// todo: make connection at con_i available again if error is encountered below
	if (con_i == -1) {
		log_this("Failed to instantiate connection: %s\n", ERROR, strerror(errno));
		return 0;
	}
	Connection * con = &(host_connections.head[con_i]);
	if (bind(socket_fd, (struct sockaddr *) (&con->addr), sizeof(struct sockaddr)) == -1) {
		log_this("Failed to bind the socket: %s\n", WARN, strerror(errno));
		close_socket(socket_fd);
		clear_connection(con);
		return 0;
	}
	log_this("Bound socket %i to port %i.\n", DEBUG, socket_fd, outgoing_port);
	if (listen(socket_fd, MAX_INCOMING_CONNECTIONS) == -1) {
		log_this("Failed to listen for connections: %s\n", WARN, strerror(errno));
		close_socket(socket_fd);
		clear_connection(con);
		return 0;
	}
	log_this("Listening for connections on port %i.\n", INFO, outgoing_port);
	return 0;
}

int handle_end_host (char ** argv) {
	return 0;
}

int handle_end_join (char ** argv) {
	return 0;
}

int handle_close (char ** argv) {
	return 0;
}

void cleanup_before_exit () {
	log_this(ANSI_COLOR_RESET, ALWAYS);
}

void handle_term_signal (int sigint) {
	cleanup_before_exit();
}

int read_user_input (char * user_input) {
	// TODO: instead of fgets, use select with a timeout?
	fgets(user_input, MAX_INPUT_LENGTH, stdin);
	// Final character is expected to be newline
	int input_length = strlen(user_input);
	if (input_length < MIN_INPUT_LENGTH) {
		log_this("Input rejected, failed to meet length requirements. Got %d, needed at least %d.\n", WARN, input_length - 1, MIN_INPUT_LENGTH - 1);
	} else {
		int last_index = input_length - 1;
		if (user_input[last_index] == '\n') {
			user_input[last_index] = '\0';
			if (user_input[0] == '/') {
				// Parse the command
				char * user_command = strtok(user_input, " ");
				for (int command_index = 0; command_index < sizeof(commands)/sizeof(commands[0]); command_index++) {
					Command known_command = commands[command_index];
					if (strcasecmp(user_command, known_command.command) == 0) {
						char * argv[MAX_PARAMS_LENGTH] = {0};
						char * word;
						int word_count = 0;
						while ((word = strtok(NULL, " "))) {
							argv[word_count] = word;
							word_count += 1;
						}
						return known_command.handler(argv);
					}
				}
				log_this("Unrecognized command '%s'. Use /help to see available commands.\n", WARN, user_command);
			} else {
				// send message to open channel(s)?
				log_this("Thank you for entering '%s'.\n", INFO, user_input);
			}
		} else {
			log_this("Input rejected, exceeded length limit. Maximum acceptable input length is %i characters.\n", WARN, MAX_INPUT_LENGTH - 1);
			while (user_input[last_index] != '\n') {
				fgets(user_input, MAX_INPUT_LENGTH, stdin);
				input_length = strlen(user_input);
				last_index = input_length - 1;
			};
		}
	}
	return 0;
}

int initialize_connections (ConnectionList * connections_list) {
	size_t length = connections_list->length;
	connections_list->head = calloc(MAX_OUTGOING_CONNECTIONS, sizeof(Connection));
	size_t i = 0;
	while (i < length) {
		connections_list->head[i] = (struct Connection){(struct sockaddr_in){AF_INET, 0, (struct in_addr){INADDR_ANY}}, -1, -1};
		i++;
	}
	log_this("Initialized %u blank connections.\n", DEBUG, i);
	return 0;
}

int main (int argc, char ** argv) {
	char user_input[MAX_INPUT_LENGTH];
	int user_input_return;
	log_this("Welcome to your %s.\n", INFO, SOFTWARE_NAME);
	if (initialize_connections(&host_connections) != 0) return EXIT_FAILURE;
	handle_help(NULL);
	// dev
	handle_host(NULL);
	//char * aport = "5555";
	//handle_host(&aport);
	handle_help(NULL);
	// /dev
	while(1 == 1) {
		user_input_return = read_user_input(user_input);
		if (user_input_return != 0) {
			break;
		};
		handle_help(NULL);
	};
	fclose(stdout);
	return EXIT_SUCCESS;
}
