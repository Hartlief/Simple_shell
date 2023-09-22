#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_COMMAND_LENGTH 100

void tokenize_input(char* input, char* tokens[], int* token_count) {
    *token_count = 0;
    char* token = strtok(input, " \t\n");

    while (token != NULL) {
        tokens[(*token_count)++] = token;
        token = strtok(NULL, " \t\n");
    }
}

int main(void) {
    char input[MAX_COMMAND_LENGTH];
    char* tokens[MAX_COMMAND_LENGTH / 2];  // Assume a reasonable limit
    int token_count;

    char* path = getenv("PATH");

    while (1) {
        printf("simple_shell$ ");  // Display the prompt

        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle end of file (Ctrl+D)
            printf("\n");
            break;
        }

        // Remove the trailing newline character
        size_t input_length = strlen(input);
        if (input_length > 0 && input[input_length - 1] == '\n') {
            input[input_length - 1] = '\0';
        }

        // Tokenize the input into command and arguments
        tokenize_input(input, tokens, &token_count);

        if (token_count == 0) {
            // No command provided
            continue;
        }

        char* command = tokens[0];

        // Check if it's the "exit" built-in command
        if (strcmp(command, "exit") == 0) {
            if (token_count == 1) {
                // No status provided, exit with status 0
                exit(0);
            } else if (token_count == 2) {
                // Status provided, we convert it to an integer using atoi
                int exit_status = atoi(tokens[1]);
                exit(exit_status);
            } else {
                fprintf(stderr, "Usage: exit [status]\n");
                continue;
            }
        }

        // Check if it's the "env" built-in command
        if (strcmp(command, "env") == 0) {
            // Print the current environment
            for (char** env = environ; *env != NULL; env++) {
                printf("%s\n", *env);
            }
            continue;
        }

        // Check if it's the "setenv" built-in command
        if (strcmp(command, "setenv") == 0) {
            if (token_count != 3) {
                fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
            } else {
                if (setenv(tokens[1], tokens[2], 1) == -1) {
                    perror("setenv");
                }
            }
            continue;
        }

        // Check if it's the "unsetenv" built-in command
        if (strcmp(command, "unsetenv") == 0) {
            if (token_count != 2) {
                fprintf(stderr, "Usage: unsetenv VARIABLE\n");
            } else {
                if (unsetenv(tokens[1]) == -1) {
                    perror("unsetenv");
                }
            }
            continue;
        }

        // Check if the command exists in one of the directories in PATH
        int command_exists = 0;
        char* path_copy = strdup(path);
        char* path_token = strtok(path_copy, ":");

        while (path_token != NULL) {
            char command_path[MAX_COMMAND_LENGTH];
            snprintf(command_path, sizeof(command_path), "%s/%s", path_token, command);

            if (access(command_path, X_OK) == 0) {
                command_exists = 1;
                break;
            }

            path_token = strtok(NULL, ":");
        }

        free(path_copy);

        if (!command_exists) {
            fprintf(stderr, "Command not found: %s\n", command);
            continue;
        }

        // Fork a child process to execute the command
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            if (execvp(command, tokens) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}

