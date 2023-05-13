#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 32
#define MAX_CMD_LENGTH 1024

int main(void) {
    char command[MAX_CMD_LENGTH];
    char *args[MAX_ARGS];
    int num_args, i;
    pid_t pid;
    int status;
    
    while (1) {
        printf("> ");
        
        /* Read a line of input from standard input */
        if (fgets(command, MAX_CMD_LENGTH, stdin) == NULL) {
            break;
        }
        
        /* Remove the newline character from the end of the command */
        command[strcspn(command, "\n")] = '\0';
        
        /* Check if the command is "exit" and exit if it is */
        if (strcmp(command, "exit") == 0) {
            break;
        }
        
        /* Parse the input into a command and its arguments */
        num_args = 0;
        args[num_args++] = strtok(command, " \t\n");
        while (num_args < MAX_ARGS - 1) {
            args[num_args] = strtok(NULL, " \t\n");
            if (args[num_args] == NULL) {
                break;
            }
            num_args++;
        }
        args[num_args] = NULL;
        
        /* Fork a new process to execute the command */
        pid = fork();
        if (pid == -1) {
            printf("Failed to fork process\n");
            continue;
        }
        else if (pid == 0) {
            /* Child process */
            if (execve(args[0], args, NULL) == -1) {
                printf("Failed to execute command\n");
                exit(1);
            }
        }
        else {
            /* Parent process */
            if (waitpid(pid, &status, 0) == -1) {
                printf("Failed to wait for child process\n");
                continue;
            }
            if (WIFEXITED(status)) {
                printf("Command exited with status %d\n", WEXITSTATUS(status));
            }
            else if (WIFSIGNALED(status)) {
                printf("Command terminated by signal %d\n", WTERMSIG(status));
            }
        }
    }
    
    return 0;
}

