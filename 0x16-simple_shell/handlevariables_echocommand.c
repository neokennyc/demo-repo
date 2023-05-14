#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void handle_echo_command(char *command) {
    char *args[20];
    int i = 0;
    int status;
if (command[strlen(command) - 1] == '"' ||
		command[strlen(command) - 1]  == '\''){
	command[strlen(command) - 1] = '\0';
	command++;
	args[i++] = "/bin/bash";
	args[i++] = "-c";
	char *echo_command = malloc(strlen(command) + 6);
	strcpy(echo_command, "echo ");
	strcat(echo_command, command); 
	args[i++] = echo_command;
}
else{
    args[i++] = "/bin/echo";  // Path to the echo command executable
    }
    char *token = strtok(command, " ");
    while (token != NULL) {
        if (strcmp(token, "$$") == 0) {
            // Process ID replacement
            char pid_str[20];
            snprintf(pid_str, sizeof(pid_str), "%d", getpid());
            args[i++] = pid_str;
        } else if (strcmp(token, "$?") == 0) {
            // Exit status replacement
            char exit_status_str[20];
            snprintf(exit_status_str, sizeof(exit_status_str), "%d", WEXITSTATUS(status));
            args[i++] = exit_status_str;
        } else if (token[0] == '$') {
            // Variable replacement
            char *variable = getenv(token + 1);  // Skip the '$' sign
            if (variable != NULL) {
                args[i++] = variable;
            }
        } else {
            // Normal argument
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }

    args[i] = NULL;

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execve(args[0], args, NULL);
        exit(0);  // Exit the child process after execve
    } else if (pid > 0) {
        // Parent process
        waitpid(pid, &status, 0);  // Wait for the child process to finish
    } else {
        // Fork failed
        printf("Fork failed\n");
    }
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

