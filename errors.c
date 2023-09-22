#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 100

int main(void) {
    char input[MAX_COMMAND_LENGTH];
    
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
        char* command = strtok(input, " ");
        char* arguments[MAX_COMMAND_LENGTH / 2];  // Assume a reasonable limit
        
        int arg_count = 0;
        while (arg_count < (MAX_COMMAND_LENGTH / 2) - 1) {
            arguments[arg_count] = strtok(NULL, " ");
            if (arguments[arg_count] == NULL) {
                break;
            }
            arg_count++;
        }
        arguments[arg_count] = NULL;  // Null-terminate the arguments list

        if (command == NULL) {
            // No command provided
            continue;
        }

        // Fork a child process to execute the command
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            if (execvp(command, arguments) == -1) {
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

