#ifndef _SHELL_H
#define _SHELL_H

#include <stddef.h>
#include <stdio.h>

#define MAX_COMMAND_LENGTH 1024

extern char **environ;

typedef struct liststr
{
    int num;
    char *str;
    struct liststr *next;
} list_t;

struct env_var {
    char *name;
    char *value;
    struct env_var *next;
};

static struct env_var *env_list = *environ;


// Prototype for the custom getline function
void interactive_shell(void);
size_t printenv(void);
char *_getenv(const char *name);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);

#endif
