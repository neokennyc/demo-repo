#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];
    char *args[128];
    int status;

    while (1) {
        printf("$ ");
        fgets(buffer, BUFFER_SIZE, stdin);

        int i = 0;
        args[i] = strtok(buffer, " \n");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " \n");
        }
        args[i] = NULL;

        if (args[0] == NULL) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            if (args[1] != NULL) {
                status = atoi(args[1]);
            } else {
                status = 0;
            }
            exit(status);
        }

        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            continue;
        } else if (pid == 0) {
            execvp(args[0], args);
            fprintf(stderr, "execvp failed\n");
            exit(1);
        } else {
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}
