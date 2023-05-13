#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */
#define MAX_ARGS MAX_LINE/2 + 1 /* The maximum number of arguments */

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    char *args[MAX_ARGS]; /* command line arguments */
    char *line = NULL; /* input line */
    size_t line_size = 0; /* size of input line */
    ssize_t line_len; /* length of input line */

    while ((line_len = getline(&line, &line_size, fp)) != -1) {
        line[line_len - 1] = '\0'; /* remove trailing newline */

        /* parse command and its arguments */
        char *token = strtok(line, " ");
        int i = 0;
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        /* execute command with arguments */
        pid_t pid = fork();
        if (pid < 0) { /* error occurred */
            fprintf(stderr, "Fork Failed");
            return 1;
        } else if (pid == 0) { /* child process */
            if (execvp(args[0], args) == -1) {
                perror("execvp");
            }
            exit(EXIT_FAILURE);
        } else { /* parent process */
            wait(NULL);
        }
    }

    free(line);
    fclose(fp);
    return 0;
}

