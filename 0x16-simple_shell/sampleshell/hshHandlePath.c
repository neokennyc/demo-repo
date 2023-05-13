#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 64
#define MAX_CMD_LEN 512

void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    char *args[MAX_ARGS];
    char cmd[MAX_CMD_LEN];
    char *path = getenv("PATH");
    char *dir;
    pid_t pid;
    int status;
    int found = 0;

    while (1) {
        printf("mysh> ");
        fflush(stdout);

        // Read command from user
        if (fgets(cmd, MAX_CMD_LEN, stdin) == NULL) {
            // End of file or error
            exit(EXIT_SUCCESS);
        }

        // Remove newline character from command
        cmd[strcspn(cmd, "\n")] = '\0';

        // Parse command into arguments
        char *token = strtok(cmd, " ");
        int i = 0;
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Check if command exists in PATH
        dir = strtok(path, ":");
        while (dir != NULL) {
            char cmd_path[MAX_CMD_LEN];
            snprintf(cmd_path, MAX_CMD_LEN, "%s/%s", dir, args[0]);
            if (access(cmd_path, F_OK) != -1) {
                found = 1;
                break;
            }
            dir = strtok(NULL, ":");
        }

        if (!found) {
            printf("%s: command not found\n", args[0]);
            continue;
        }

        // Execute command
        pid = fork();
        if (pid == -1) {
            error("fork");
        } else if (pid == 0) {
            // Child process
            execvp(args[0], args);
            // If execvp returns, then the command was not found
            printf("%s: command not found\n", args[0]);
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}


