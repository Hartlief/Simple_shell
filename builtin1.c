#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

/**
 * main - Entry point
 * Description: Use the command "exit" using "strcmp", if it is we break out of the shell loop, effectively exiting the shell.
 * Return: 0.
 */

int main(void)
{
char input[MAX_COMMAND_LENGTH];
char *path = getenv("PATH");
    
    while (1) {
        printf("simple_shell$ ");  Display the prompt
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            Handle end of file (Ctrl+D)
            printf("\n");
            break;
        }
        
        Remove the trailing newline character
        size_t input_length = strlen(input);
        if (input_length > 0 && input[input_length - 1] == '\n') {
            input[input_length - 1] = '\0';
        }

        Tokenize the input into command and arguments
        char* command = strtok(input, " ");
        char* arguments[MAX_COMMAND_LENGTH / 2];  Assume a reasonable limit

/**
 * main - The rest of the code remains unchanged, and it continues to handle the other commands as before.
 *
 * Return: 0.
 */
int main (void)
{
int arg_count = 0;
while (arg_count < (MAX_COMMAND_LENGTH / 2) - 1) {
       arguments[arg_count] = strtok(NULL, " ");
       if (arguments[arg_count] == NULL) {
           break;
            }
            arg_count++;
        }
        arguments[arg_count] = NULL;  Null-terminate the arguments list

        if (command == NULL) {
            No command provided
            continue;
        }

        Check if it's the "exit" built-in command
        if (strcmp(command, "exit") == 0) {
            Exit the shell
            break;
        }

        Check if the command exists in one of the directories in PATH
        int command_exists = 0;
        char *path_copy = strdup(path);
        char *path_token = strtok(path_copy, ":");
        
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

        Fork a child process to execute the command
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            Child process
            if (execvp(command, arguments) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {
            Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}

