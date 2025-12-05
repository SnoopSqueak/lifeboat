// Mike Stowe 2025
// https://snoopsqueak.com

#include "lbui.h"
#include "conlist.h"
#include "logger.h"
#define SOFTWARE_NAME "LifeBoat"

#define MAX_INPUT_LENGTH 2048
#define MIN_INPUT_LENGTH 2
#define MAX_PARAMS_LENGTH 32
//#define MAX_IN_CONS 64
#define MAX_IN_CONS 4
//#define MAX_OUT_CONS 64
#define MAX_OUT_CONS 4
#define DEF_PORT 25252

//~ int handle_quit (char ** argv) {
        //~ log_this(log_level, "Exiting program.\n", LL_DEBUG);
        //~ return 1;
//~ }

/*
 * uint16_t outgoing_port = DEF_PORT;
        if (argv != NULL) {
                char * port_string = argv[0];
                if (port_string != NULL && strlen(port_string) > 0) {
                        outgoing_port = strtoul(port_string, NULL, 10);
                }
        }
 */

int handle_help (char ** argv) {
        //~ struct Connection * active_connections = calloc(host_connections.length, sizeof(struct Connection));
        //~ size_t con_count = get_active_connections(&host_connections, active_connections);
        //~ if (con_count > 0) {
                //~ log_this("%u active host socket(s):\n  ", INFO, con_count);
                //~ for (size_t con_i = 0; con_i < con_count; con_i++) {
                        //~ if (con_i > 0) {
                                //~ log_this(", ", INFO);
                        //~ }
                        //~ log_this(AC_BR_BLUE "#%i: %u:%u" AC_RESET, INFO, active_connections[con_i].id, ntohs(active_connections[con_i].addr.sin_addr.s_addr), ntohs(active_connections[con_i].addr.sin_port));
                //~ }
                //~ log_this(".\n", INFO);
        //~ } else {
                //~ log_this("(Not acting as host.)\n", INFO);
        //~ }
        //~ free(active_connections);
        //~ active_connections = calloc(client_connections.length, sizeof(struct Connection));
        //~ con_count = get_active_connections(&client_connections, active_connections);
        //~ if (con_count > 0) {
                //~ log_this("%u active client socket(s):\n  ", INFO, con_count);
                //~ for (size_t con_i = 0; con_i < con_count; con_i++) {
                        //~ if (con_i > 0) {
                                //~ log_this(", ", INFO);
                        //~ }
                        //~ log_this(AC_BR_CYAN "%i %u" AC_RESET, INFO, active_connections[con_i].id, ntohs(active_connections[con_i].addr.sin_port));
                //~ }
                //~ log_this(".\n", INFO);
        //~ } else {
                //~ log_this("(Not acting as client.)\n", INFO);
        //~ }
        //~ free(active_connections);
        //log_this(log_level, "Available commands:\n", LL_INFO);
        //~ int commands_length = sizeof(commands)/sizeof(commands[0]);
        //~ for (int i = 0; i < commands_length; i++) {
                //~ log_this(log_level, "  %s %s\n", LL_INFO, commands[i].command, commands[i].about);
        //~ }
        return 0;
}

//~ int handle_close (char ** argv) {
//~ if (argv == NULL || argv[0] == NULL) {
//~ log_this("Please provide a connection ID. Example: /close 0\n", WARN);
//~ return 0;
//~ }
//~ ssize_t con_id = -1;
//~ char * in_string = argv[0];
//~ //size_t length = strlen(argv[0]);
//~ //char in_string_low[length];
//~ //for (uint16_t i = 0; i <= length; i++) {
//~ //in_string_low[i] = tolower(in_string[i]);
//~ //}
//~ //if (in_string_low == "all") {
//~ //// get all active connections
//~ //// close each one
//~ //log_this("Not yet implemented. TODO, WIP, etc.\n", ERROR);
//~ //return 0;
//~ //}
//~ if (in_string != NULL && strlen(in_string) > 0) {
//~ con_id = atoi(in_string);
//~ }
//~ if (con_id < 0) {
//~ log_this("Failed to parse connection ID from input.\n", ERROR);
//~ return -1;
//~ }
//~ struct Connection * con = get_any_con_by_id(con_id);
//~ if (con != NULL) {
//~ clear_con_close_sock(con);
//~ }
//~ return 0;
//~ }

//~ int read_user_input (char * user_input) {
        //~ // TODO: instead of fgets, use select with a timeout?
        //~ fgets(user_input, MAX_INPUT_LENGTH, stdin);
        //~ // Final character is expected to be newline
        //~ int input_length = strlen(user_input);
        //~ if (input_length < MIN_INPUT_LENGTH) {
                //~ log_this(log_level, "Input rejected, failed to meet length requirements. Got %d, needed at least %d.\n", LL_WARN, input_length - 1, MIN_INPUT_LENGTH - 1);
        //~ } else {
                //~ int last_index = input_length - 1;
                //~ if (user_input[last_index] == '\n') {
                        //~ user_input[last_index] = '\0';
                        //~ if (user_input[0] == '/') {
                                //~ // Parse the command
                                //~ char * user_command = strtok(user_input, " ");
                                //~ for (int command_index = 0; command_index < sizeof(commands)/sizeof(commands[0]); command_index++) {
                                        //~ struct Command known_command = commands[command_index];
                                        //~ if (strcasecmp(user_command, known_command.command) == 0) {
                                                //~ char * argv[MAX_PARAMS_LENGTH] = {0};
                                                //~ char * word;
                                                //~ int word_count = 0;
                                                //~ while ((word = strtok(NULL, " "))) {
                                                        //~ argv[word_count] = word;
                                                        //~ word_count += 1;
                                                //~ }
                                                //~ return known_command.handler(argv);
                                        //~ }
                                //~ }
                                //~ log_this(log_level, "Unrecognized command '%s'. Use /help to see available commands.\n", LL_WARN, user_command);
                        //~ } else {
                                //~ // send message to open channel(s)?
                                //~ log_this(log_level, "Thank you for entering '%s'.\n", LL_INFO, user_input);
                        //~ }
                //~ } else {
                        //~ log_this(log_level, "Input rejected, exceeded length limit. Maximum acceptable input length is %i characters.\n", LL_WARN, MAX_INPUT_LENGTH - 1);
                        //~ while (user_input[last_index] != '\n') {
                                //~ fgets(user_input, MAX_INPUT_LENGTH, stdin);
                                //~ input_length = strlen(user_input);
                                //~ last_index = input_length - 1;
                        //~ };
                //~ }
        //~ }
        //~ return 0;
//~ }

int main (int argc, char ** argv) {
        //~ struct conlist * hostcons = make_conlist(MAX_IN_CONS);
        //~ struct command commands[] = {
                //~ {"/quit", "Exit this program.", handle_quit},
                //~ {"/host",
                //~ "[port]  Open your " SOFTWARE_NAME " for others to join the"
                //~ " lobby on port [port].", handle_host},
                //~ {"/close",
                //~ "[connection id]  Close the connection that matches the"
                //~ " given [connection ID]. \"/close all\" to close all "
                //~ "connections.", handle_close}
        //~ };
        //~ char user_input[MAX_INPUT_LENGTH];
        //~ int user_input_return;
        struct logger * lb_logger = make_logger(LL_NEVER, stdout, CF_ANSI);
        log_this(lb_logger, "Welcome to your %s.\n", LL_INFO, SOFTWARE_NAME);
        //~ while(1 == 1) {
                handle_help(NULL);
                //user_input_return = read_user_input(user_input);
                //~ if (user_input_return != 0) {
                        //~ break;
                //~ };
        //~ };
        return EXIT_SUCCESS;
}
