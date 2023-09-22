#include "shell.h"

/**
 * main - Function to check if a command exists in one of the directories in PATH
 *
 * Return: 0.
 */
char *_getenv(const char *name);
{
    char *path = getenv("PATH");
    if (path == NULL) {
        return 0;
    }

    char *path_copy = strdup(path);
    char *token = strtok(path_copy, ":");
    while (token != NULL)
    {
        char cmd_path[MAX_INPUT_SIZE];
        snprintf(cmd_path, sizeof(cmd_path), "%s/%s", token, command);

        if (access(cmd_path, X_OK) == 0)
        {
            free(path_copy);
            return 1;
        }

        token = strtok(NULL, ":");
    }

    free(path_copy);
    return 0;
}

int main()
{
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    int status;

    while (1)
    {
        printf("$ ");
        flush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
	{
           printf("\nExiting the shell.\n");
            break;
        }

        int arg_count = 0;
        char *token = strtok(input, " \t\n");
        while (token != NULL)
	{
            args[arg_count] = token;
            arg_count++;
            token = strtok(NULL, " \t\n");

            if (arg_count >= MAX_ARGS)
	    {
                fprintf(stderr, "Too many arguments.\n");
                break;
            }
        }
        args[arg_count] = NULL;

        if (arg_count > 0)
	{
            if (command_exists(args[0]))
	    {
                pid_t pid = fork();
                if (pid < 0) {
                    perror("Fork failed");
                    exit(1);
                } else if (pid == 0) {
                    Child process
                    if (execvp(args[0], args) == -1) {
                        perror("Command execution failed");
                        exit(1);
                    }
                } else {
                  
                    wait(&status);
                }
            } else {
                fprintf(stderr, "Command not found: %s\n", args[0]);
            }
        }
    }

    return 0;
}

