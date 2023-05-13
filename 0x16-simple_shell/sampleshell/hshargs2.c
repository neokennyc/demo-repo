#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */
#define MAX_ARGS MAX_LINE/2 + 1 /* The maximum number of arguments */

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *args[MAX_ARGS]; /* command line arguments */

    /* parse command and its arguments */
    int i;
    for (i = 1; i < argc; i++) {
        args[i-1] = argv[i];
    }
    args[i-1] = NULL;

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

    return 0;
}

