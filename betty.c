/**
 * print_hello - Prints "Hello, Betty!" to the standard output
 *
 * Description: This function prints a greeting message to the console.
 * It is a demonstration of code that adheres to the Betty coding style.
 */
void print_hello(void)
{
char *message = "Hello, Betty!";

    /*
     * Use puts() to print the message
     * as it automatically appends a newline character.
     */
puts(message);
}

/**
 * main - Entry point of the program
 *
 * Description: This is the main function that initializes the program.
 * It calls the print_hello() function to print a greeting message.
 *
 * Return: 0 (Success)
 */
int main(void)
{
print_hello();
return (0);
}

