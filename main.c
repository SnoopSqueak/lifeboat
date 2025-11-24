// Mike Stowe 2025
// https://snoopsqueak.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 4
#define MIN_INPUT_LENGTH 2

void read_user_input (char* user_input) {
	fgets(user_input, MAX_INPUT_LENGTH, stdin);
	// Final character is expected to be newline
	int input_length = strlen(user_input);
	if (input_length < MIN_INPUT_LENGTH) {
		printf("Input rejected, failed to meet length requirements. Got %d, needed at least %d.\n", input_length - 1, MIN_INPUT_LENGTH - 1);
	} else {
		int last_index = input_length - 1;
		if (user_input[last_index] == '\n') {
			user_input[last_index] = '\0';
			printf("Thank you for entering '%s'.\n", user_input);
		} else {
			printf("Input rejected, exceeded length limit. Maximum acceptable input length is %i characters.\n", MAX_INPUT_LENGTH - 1);
		}
	}
}

int main (int argc, char **argv) {
	char user_input[MAX_INPUT_LENGTH];
	char *output_file_path;
	int argi;
	opterr = 0;
	while ((argi = getopt (argc, argv, "o:")) != -1) {
		switch (argi) {
			case 'o':
				output_file_path = optarg;
			break;
			case '?':
				if (optopt == 'o') {
					fprintf(stderr, "Option -%o requires an argument.\n", optopt);
				} else {
					fprintf(stderr, "Unknown option.\n");
				}
			break;
			default: abort();
		}
	}
	if (output_file_path) {
		printf("Redirecting output to %s...\n", output_file_path);
		if (freopen(output_file_path, "w", stdout) == NULL) {
			printf("Failed to open %s for writing.\n", user_input);
		}
	} else {
		printf("No output redirection detected. Printing to console.\n");
	}
	printf("Welcome to your very own LifeBoat.\n");
	printf("Enter anything to exit.\n");
	while(1 == 1) {
		read_user_input(user_input);
		break;
	}
	printf("Well done! Farewell.\n");
	fclose(stdout);
	return EXIT_SUCCESS;
}
