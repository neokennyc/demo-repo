#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int last_exit_code = 0;

char* handle_variables(char* command) {
    char* result = malloc(strlen(command) + 1);
    int i = 0, j = 0;
    while (command[i] != '\0') {
        if (command[i] == '$' && command[i + 1] == '$') {
            pid_t pid = getpid();
            char pid_str[20];
            sprintf(pid_str, "%d", pid);
            strcat(result, pid_str);
            i += 2;
            j += strlen(pid_str);
        } else if (command[i] == '$' && command[i + 1] == '?') {
            char exit_code_str[20];
            sprintf(exit_code_str, "%d", last_exit_code);
            strcat(result, exit_code_str);
            i += 2;
            j += strlen(exit_code_str);
        } else {
            result[j] = command[i];
            i++;
            j++;
        }
    }
    result[j] = '\0';
    return result;
}

void execute_command(char* command) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        char* args[] = {"/bin/sh", "-c", command, NULL};
        execve(args[0], args, NULL);
        perror("execve");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) {
            last_exit_code = WEXITSTATUS(status);
        }
    }
}

void interactive() {
    char command[1000];
    char* modified_command;

    while (1) {
        printf("$ ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';  // Remove trailing newline character

        if (strcmp(command, "exit") == 0) {
            break;
        }

        modified_command = handle_variables(command);
        execute_command(modified_command);
        free(modified_command);
    }
}

int main() {
    interactive();

    return 0;
}

