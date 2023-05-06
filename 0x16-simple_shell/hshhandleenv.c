#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

void shell_env(void) {
    char **envp = environ;
    while (*envp != NULL) {
        printf("%s\n", *envp);
        envp++;
    }
}

int main(void) {
    char input[1024];
    char *args[64];
    int status = 1;

    while (status) {
        printf("$ ");
        fgets(input, 1024, stdin);

        // Tokenize the input
        int i = 0;
        args[i] = strtok(input, " \n");
        while (args[i] != NULL && i < 63) {
            i++;
            args[i] = strtok(NULL, " \n");
        }
        args[i] = NULL;

        // Check for built-in commands
        if (strcmp(args[0], "env") == 0) {
            shell_env();
        } else if (strcmp(args[0], "exit") == 0) {
            status = 0;
        } else {
            // Execute external command
            pid_t pid = fork();
            if (pid == 0) {
                execvp(args[0], args);
                perror("execvp");
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                wait(NULL);
            } else {
                perror("fork");
                exit(EXIT_FAILURE);
            }
        }
    }

    return EXIT_SUCCESS;
}

