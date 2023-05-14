#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void handle_echo_command(char *command) {
    int has_quotes = 0;
    if (command[0] == '"' && command[strlen(command) - 1] == '"') {
        has_quotes = 1;
        command[strlen(command) - 1] = '\0';
        command++;
    }

    char *args[20];
    int i = 0;
    args[i++] = "/bin/bash";  // Path to the bash interpreter
    args[i++] = "-c";  // Run a command through the bash interpreter

    char *echo_command = malloc(strlen(command) + 6);  // Allocate memory for echo command
    strcpy(echo_command, "echo ");  // Start with "echo "
    strcat(echo_command, command);  // Append the command

    if (has_quotes) {
        // Quote handling with bash -c
        args[i++] = echo_command;
    } else {
        // No quotes, execute the command directly
        args[i++] = command;
    }

    args[i] = NULL;

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execve(args[0], args, NULL);
        exit(0);  // Exit the child process after execve
    } else if (pid > 0) {
        // Parent process
        waitpid(pid, NULL, 0);  // Wait for the child process to finish
    } else {
        // Fork failed
        printf("Fork failed\n");
    }

    free(echo_command);  // Free allocated memory
}

int main() {
    char command[100];

    while (1) {
        printf("$ ");
        fgets(command, sizeof(command), stdin);

        // Remove trailing newline character
        command[strcspn(command, "\n")] = '\0';

        if (strncmp(command, "echo ", 5) == 0) {
            handle_echo_command(command + 5);  // Skip "echo " prefix
        }
else if (strcmp(command, "exit") == 0) {
            exit(0);
        }
    }

    return 0;
}

