#ifndef _MAIN_H
#define _MAIN_H

#include <stddef.h>
#include <stdio.h>

#define MAX_COMMAND_LENGTH 1024
// Prototype for the custom getline function
void interactive_shell(void);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);

#endif
