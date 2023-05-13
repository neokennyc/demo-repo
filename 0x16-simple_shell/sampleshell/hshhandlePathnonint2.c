#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_NUM_ARGUMENTS 64

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    // Declare the args array
    char *args[MAX_NUM_ARGUMENTS + 1];

    // Copy the command line arguments into the args array
    int i;
    for (i = 1; i < argc && i <= MAX_NUM_ARGUMENTS; i++) {
        args[i-1] = argv[i];
    }
    args[i-1] = NULL;

    // Look for the command in the PATH and execute it
    char *path = getenv("PATH");
    if (path == NULL) {
        fprintf(stderr, "PATH not set\n");
        return 1;
    }
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    while (dir != NULL) {
        char cmd_path[MAX_COMMAND_LENGTH];
        snprintf(cmd_path, sizeof(cmd_path), "%s/%s", dir, args[0]);
        if (access(cmd_path, X_OK) == 0) {
            // Command found, execute it
            pid_t child_pid = fork();
            if (child_pid == -1) {
                perror("fork");
                return 1;
            }
            if (child_pid == 0) {
                // Child process
                execv(cmd_path, args);
                perror("execv");
                exit(1);
            } else {
                // Parent process
                int status;
                waitpid(child_pid, &status, 0);
                return WEXITSTATUS(status);
            }
        }
        dir = strtok(NULL, ":");
    }
    fprintf(stderr, "%s: command not found\n", args[0]);
    return 1;
}

