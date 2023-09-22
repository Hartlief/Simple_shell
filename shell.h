#ifndef SHELL_H
#define SHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stddef.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

int main(void);
int main(int ac, char **av);
int main(int ac, char **av, char **env);
char *_getenv(const char *name);
int _unsetenv(const char *name);
int _setenv(const char *name, const char *value, int overwrite);

#endif

