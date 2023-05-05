#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

int main(int argc, char **argv) {
    char command[MAX_COMMAND_LENGTH];
    FILE *input = stdin;

    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[1]);
            exit(1);
        }
    }

    while (fgets(command, MAX_COMMAND_LENGTH, input) != NULL) {
        // Remove trailing newline character
        command[strcspn(command, "\n")] = '\0';

        // Split the command into its arguments
        char *args[MAX_COMMAND_LENGTH];
        int i = 0;
        char *token = strtok(command, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Execute the command
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            execve(args[0], args, NULL);
            fprintf(stderr, "%s: %d: %s: not found\n", argv[0], 1, args[0]);
            exit(1);
        } else if (pid < 0) {
            fprintf(stderr, "%s: Failed to fork process\n", argv[0]);
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
            if (status != 0) {
                fprintf(stderr, "%s: %d: %s: exited with status %d\n", argv[0], pid, command, status);
            }
        }
    }

    if (input != stdin) {
        fclose(input);
    }

    return 0;
}

