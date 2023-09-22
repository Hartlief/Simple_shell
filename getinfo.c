
#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

/**
 * main - Prints a code that runs the executable to create a simple shell that supports these logical operators
 *
 * Return: Always 0 (Success)
 */
int execute_command(char* args[])
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) == -1)
        {
            perror("Shell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("Shell");
    } else
    {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return WEXITSTATUS(status);
}

int main()
{
    char input[MAX_INPUT_SIZE];
    char* args[MAX_ARGS];
    char current_directory[MAX_INPUT_SIZE];

    while (1)
    {
  {
        // Get the current working directory
        if (getcwd(current_directory, sizeof(current_directory)) == NULL)
        {
            perror("getcwd");
            break;
        }

        printf("%s$ ", current_directory);
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break; // End of input or error
        }

        int success = 1; // Flag to track success or failure
        char* command = strtok(input, ";\n");
        while (command != NULL)
        {
            // Tokenize each command into arguments
            int arg_count = 0;
            char* token = strtok(command, " \t\n");
            while (token != NULL)
            {
                args[arg_count] = token;
                arg_count++;
                token = strtok(NULL, " \t\n");
            }
            args[arg_count] = NULL;

            if (arg_count > 0)
            {
                // Check if the command is "cd"
                if (strcmp(args[0], "cd") == 0)
                {
                    // Handle "cd" command as before
                    // ...
                }
                // Handle "&&" operator
                else if (strcmp(args[arg_count - 1], "&&") == 0)
                {
                    args[arg_count - 1] = NULL; // Remove "&&"
                    if (success) {
                        success = execute_command(args) == 0;
                    }
                }
                // Handle "||" operator
                else if (strcmp(args[arg_count - 1], "||") == 0)
               {
                    args[arg_count - 1] = NULL; // Remove "||"
                    if (!success)
                    {
                        success = execute_command(args) == 0;
                    }
                }
                // Execute other commands
                else
                {
                    success = execute_command(args) == 0;
                }
            }

            // Get the next command
            command = strtok(NULL, ";\n");
        }
    }

    return 0;
}

