#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define INITIAL_BUFFER_SIZE 128

static char* buffer = NULL;
static size_t buffer_size = 0;
static size_t buffer_position = 0;

ssize_t custom_getline(char** lineptr, size_t* n, FILE* stream) {
    if (lineptr == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (buffer == NULL) {
        buffer = (char*)malloc(INITIAL_BUFFER_SIZE);
        if (buffer == NULL) {
            return -1;
        }
        buffer_size = INITIAL_BUFFER_SIZE;
        buffer_position = 0;
    }

    ssize_t bytesRead = 0;
    int character;

    while (1)
    {
        if (buffer_position >= buffer_size - 1) {
            size_t new_size = buffer_size * 2;
            char* new_buffer = (char*)realloc(buffer, new_size);
            if (new_buffer == NULL) {
                return -1; 
            }
            buffer = new_buffer;
            buffer_size = new_size;
	}
        character = fgetc(stream);

        if (character == EOF)
	{
            if (bytesRead == 0)
	    {
                return -1;
            } else 

                buffer[buffer_position] = '\0';
                *lineptr = buffer;
                *n = buffer_position;
                return bytesRead;
            }
        } else
	{
            buffer[buffer_position++] = (char)character;
            bytesRead++;

            if (character == '\n') {
                buffer[buffer_position] = '\0';
                *lineptr = buffer;
                *n = buffer_position;
                return bytesRead;
            }
        }
    }
}

int main(void)
{
    char* line = NULL;
    size_t line_length = 0;
    
    while (1)
    {
        printf("Enter a line: ");
        ssize_t read = custom_getline(&line, &line_length, stdin);
        
        if (read == -1)
	{
            printf("Error reading input\n");
            break;
        }
        
        if (read == 0)
	{
            printf("End of input\n");
            break;
        }
        
        printf("You entered: %s", line);
    }
    
    free(line);
    return 0;
}

