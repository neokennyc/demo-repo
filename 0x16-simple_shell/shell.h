#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>


/**
 * struct command - contains a shell commands  string and related function
 * @type: the  command 
 * @func: the function call
 */
typedef struct command
{
	char *type;
/*	int (*func)(info_t *);*/
} command_table;


#define MAX_COMMAND_LENGTH 1024
#define MAX_ALIAS_NAME 1024
#define BUFFER_SIZE 1024
#define MAX_ALIAS_VALUE 1024
#define ALIAS_FILE "aliases.txt"
#define MAX_NUM_ARGS 1024
extern char **environ;

typedef struct liststr
{                                             int num;
    char *str;
    struct liststr *next;
} list_t;

struct env_var {
    char *name;
    char *value;
    struct env_var *next;
};

static struct env_var *env_list = NULL;
/**
 * struct stdinfo - ====
 * @arg: a string generated from getline containing arguements
 * @argv: an array of strings generated from arg
 * @path: a string path for the current command
 * @argc: the argument count
 *@environ: +====
 *@status: =+====
 */
typedef struct stdinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	char **environ;
	int status;
} stdinfo_t;

#define STD_INIT \
    { \
        NULL, /* ptr1 */ \
        NULL, /* ptr2 */ \
        NULL, /* ptr3 */ \
        0,    /* val */ \
        NULL, /* ptr4 */ \
        0     /* val2 */ \
    }


// Prototype for the custom getline function
char *_strrchr(char *str, int character);
int _rename(const char *oldname, const char *newname);
char *_strstr(char *haystack, char *needle);
char *_strncpy(char *dest, char *src, int n);
int _strncmp(char *s1, char *s2, size_t n);
char *_strdup(char *str);
char *_strstr(char *haystack, char *needle);
int is_delim(char c, char *delim);
char *_strchr(char *str, char ch);
char *_strtok(char *str, char *delimiters);
char **_strtow(char *str, char *d);
void _writef(const char *format, ...);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
int _strcmp(char *s1, char *s2);
int _strlen(char *s);
void interactive_shell(void);
int populate_env_list(list_t **head);
size_t printenv(void);
char *_getenv(const char *name);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);

#endif
