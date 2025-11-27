// Mike Stowe 2025
// https://snoopsqueak.com

#include <errno.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 2048
#define MIN_INPUT_LENGTH 2
#define MAX_PARAMS_LENGTH 32
#define MAX_CONNECTIONS 128
#define CONNECTIONS_BATCH_LENGTH 8
#define SOFTWARE_NAME(n) "LifeBoat"

#define DEFAULT_PORT 25252

//const char * SOFTWARE_NAME = "LifeBoat";
int connections_batch_index = 0;

typedef struct {
	char * command;
	char * about;
	int (* handler)(char ** argv);
} Command;

typedef struct {
	char * address;
	int sockfd;
} Connection;

int handle_quit (char ** argv) {
	printf("Exiting program.\n");
	return 1;
}

int handle_help(char ** argv);

int handle_connect(char ** argv);

Command commands[] = {
	{"/help", "List available commands.", handle_help},
	{"/quit", "Exit this program.", handle_quit},
	{"/connect", "Connect to another " SOFTWARE_NAME() ".", handle_connect}
};

Connection connections[CONNECTIONS_BATCH_LENGTH];
int next_connection_index = 0;

int handle_help (char ** argv) {
	printf("Available commands:\n");
	int commands_length = sizeof(commands)/sizeof(commands[0]);
	for (int i = 0; i < commands_length; i++) {
		Command command = commands[i];
		printf("  %s  %s\n", command.command, command.about);
	}
	return 0;
}

int make_connection (char * address) {
	printf("Attempting to connect to %s...\n", address);
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		printf("Failed to open the socket: %i", errno);
		return 0;
	}
	Connection connection = {address, socket_fd};
	connections[next_connection_index] = connection;
	next_connection_index += 1;
	// TODO: resize connections array when too many connected
	//if (((1 + connections_batch_index) * CONNECTIONS_BATCH_LENGTH) <= next_connection_index) {
		//connections_batch_index += 1;
	//}
	printf("Connected! Socket fd is %i.\n", socket_fd);
	return 0;
}

int handle_connect (char ** argv) {
	char * address = argv[0];
	make_connection(address);
	return 0;
}

int read_user_input (char * user_input) {
	fgets(user_input, MAX_INPUT_LENGTH, stdin);
	// Final character is expected to be newline
	int input_length = strlen(user_input);
	if (input_length < MIN_INPUT_LENGTH) {
		printf("Input rejected, failed to meet length requirements. Got %d, needed at least %d.\n", input_length - 1, MIN_INPUT_LENGTH - 1);
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
				printf("Unrecognized command '%s'. Use /help to see available commands.\n", user_command);
			} else {
				// send message to open channel(s)?
				printf("Thank you for entering '%s'.\n", user_input);
			}
		} else {
			printf("Input rejected, exceeded length limit. Maximum acceptable input length is %i characters.\n", MAX_INPUT_LENGTH - 1);
		}
	}
	return 0;
}

int main (int argc, char ** argv) {
	char user_input[MAX_INPUT_LENGTH];
	char * output_file_path = NULL;
	char * input_port = NULL;
	int argi;
	int user_input_return;
	opterr = 0;
	while ((argi = getopt (argc, argv, "o:")) != -1) {
		switch (argi) {
			case 'p':
				input_port = optarg;
			break;
			case 'o':
				output_file_path = optarg;
			break;
			case '?':
				if (optopt == 'o' || optopt == 'p') {
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				} else {
					fprintf(stderr, "Unknown option.\n");
				}
			break;
			default: abort();
		}
	}
	if (output_file_path != NULL) {
		printf("Redirecting output to %s...\n", output_file_path);
		if (freopen(output_file_path, "w", stdout) == NULL) {
			printf("Failed to open %s for writing.\n", user_input);
		}
	} else {
		printf("No output redirection detected. Printing to console.\n");
	}
	if (input_port != NULL) {
		// use given port
	} else {
		// use default port
	}
	printf("Welcome to your very own %s.\n", SOFTWARE_NAME());
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
