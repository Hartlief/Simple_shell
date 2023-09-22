#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 100

int main(void) {
    char command[MAX_COMMAND_LENGTH];
    while (1) {
        printf("simple_shell$ ");  // Display the prompt
        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
            // Handle end of file (Ctrl+D)
            printf("\n");
            break;
        }
        
        // Remove the trailing newline character
        size_t command_length = strlen(command);
        if (command_length > 0 && command[command_length - 1] == '\n') {
            command[command_length - 1] = '\0';
        }
        
        // Execute the command
        int result = system(command);
        if (result == -1) {
            // Handle errors
            fprintf(stderr, "Error executing the command.\n");
        }
    }
    return 0;
}

