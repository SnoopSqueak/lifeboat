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
#define MAX_OUTGOING_CONNECTIONS 64
#define SOFTWARE_NAME(n) "LifeBoat"

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

int connections_batch_index = 0;
char * socket_path = "./sockets/";

typedef struct {
	char * command;
	char * about;
	int (* handler)(char ** argv);
} Command;

typedef struct {
	char * address;
	int sockfd;
} Connection;

Connection host_connections[MAX_OUTGOING_CONNECTIONS];
Connection client_connections[MAX_INCOMING_CONNECTIONS];

void log_this (char * message, int message_log_level, ...) {
	if (message_log_level > NEVER && log_level <= message_log_level) {
		va_list args;
		va_start(args, message_log_level);
		char * color;
		switch (message_log_level) {
			case ALWAYS: color = ANSI_COLOR_BRIGHT_WHITE; break;
			case TRACE: color = ANSI_COLOR_BRIGHT_BLACK; break;
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

int make_connection(int socket_fd, struct sockaddr_in addr_info, Connection connections_list[]);

int handle_dev(char ** argv);

Command commands[] = {
	{"/help", "List available commands.", handle_help},
	{"/quit", "Exit this program.", handle_quit},
	{"/host", "[port]  Open your " SOFTWARE_NAME() " for others to join the lobby on port [port].", handle_host},
	{"/join", "[address]  Connect to another " SOFTWARE_NAME() " that is hosting at [address].", handle_join},
	{"/close", "[connection_number]  Close all open connections.", handle_close},
	{"/say", "[connection_number]  Send a message through the given channel(s).", handle_close},
	{"/dev", "(Scrap function for debug and development.)", handle_dev}
};

int make_connection (int socket_fd, struct sockaddr_in addr_info, Connection connections_list[]) {
	int i = 0;
	int length = sizeof(&connections_list)/sizeof(Connection);
	log_this("Found %i connections.\n", TRACE, length);
	Connection existing;
	while (i < length) {
		existing = connections_list[i];
		if (existing.sockfd == socket_fd) {
			log_this("Given socket %i already exists as a known connection.\n", WARN, socket_fd);
			return -1;
		}
		i++;
	}
	if (i < length - 1) {
		getsockname(socket_fd, addr_info, 24);
		log_this("Socket address: %s\n", DEBUG, socket_address);
		connections_list[i] = Connection(socket_fd);
	}
	return 0;
}

int handle_dev(char ** argv) {
	char * port[0];
	handle_host(port);
	//make_connection(3, host_connections);
	log_this("Done?\n", DEBUG);
	return 0;
}

int handle_help (char ** argv) {
	log_this("Available commands:\n", INFO);
	int commands_length = sizeof(commands)/sizeof(commands[0]);
	for (int i = 0; i < commands_length; i++) {
		Command command = commands[i];
		log_this("  %s  %s\n", INFO, command.command, command.about);
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
	char * port_string = argv[0];
	int outgoing_port = DEFAULT_PORT;
	if (port_string != NULL && strlen(port_string) > 0) {
		outgoing_port = strtol(port_string, NULL, 10);
	}
	log_this("Opening INADDR_ANY on port %i...\n", DEBUG, outgoing_port);
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		log_this("Failed to open the socket: %s\n", WARN, strerror(errno));
		return 0;
	}
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(outgoing_port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
		log_this("Failed to bind the socket: %s\n", WARN, strerror(errno));
		return 0;
	}
	log_this("Bound socket %i to port %i.\n", DEBUG, socket_fd, outgoing_port);
	if (listen(socket_fd, MAX_INCOMING_CONNECTIONS) == -1) {
		log_this("Failed to listen for connections: %s\n", WARN, strerror(errno));
		return 0;
	}
	//if available:
	make_connection(sockfd, server_address.sock_addr, host_connections);
	//else, close socket, error and return.
	log_this("Listening for connections on port %i.\n", INFO, outgoing_port);
	return 0;
}

int handle_end_host (char ** argv) {
	/*if (host_connection.sockfd != -1) {
		if (close(host_connection.sockfd) != -1) {
			log_this("Closed connection to socket %i.\n", DEBUG, host_connection.sockfd);
			host_connection.sockfd = -1;
		} else {
			log_this("Error closing connection: %s\n", ERROR, strerror(errno));
			return 1;
		}
		log_this("Done hosting.\n", INFO);
	}*/
	return 0;
}

int handle_end_join (char ** argv) {
	log_this("/endjoin not yet implemented. But neither is /join, so meh.\n", WARN);
	return 0;
}

int handle_close (char ** argv) {
	handle_end_host(argv);
	handle_end_join(argv);
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

int main (int argc, char ** argv) {
	char user_input[MAX_INPUT_LENGTH];
	int user_input_return;
	log_this("Welcome to your very own %s.\n", INFO, SOFTWARE_NAME());
	handle_help(NULL);
	while(1 == 1) {
		user_input_return = read_user_input(user_input);
		if (user_input_return != 0) {
			break;
		};
	};
	fclose(stdout);
	return EXIT_SUCCESS;
}
