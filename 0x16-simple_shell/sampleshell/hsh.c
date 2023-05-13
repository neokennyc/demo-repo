#include <stdio.h>
#include "main.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void interactive_shell() 
{
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_COMMAND_LENGTH/2 + 1];
    int should_run = 1;
    char *token;
    int status, j, i = 0;

    while (should_run) 
    {
        printf("($): ");
        fflush(stdout);
        fgets(command, MAX_COMMAND_LENGTH, stdin);
        command[strcspn(command, "\n")] = 0;
        token = strtok(command, " ");
        while (token != NULL) 
	{
            if (token[0] == '-') 
	    {
                for (j = 1; j < strlen(token); j++) {
                    args[i++] = &token[j];
                }
            } else {
                // This argument is not an option flag
                args[i++] = token;
            }
            token = strtok(NULL, " ");
        }
        args[i] = NULL;
        if (strcmp(args[0], "exit") == 0) 
	{
            should_run = 0;
            break;
        }
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Failed to fork()\n");
            exit(1);
        } 
	else if (pid == 0) 
	{
		if (execvp(args[0], args) < 0) 
		{
                fprintf(stderr, "Failed to execute command: %s\n", args[0]);
                exit(1);
            }
        } 
	else 
	{
            waitpid(pid, &status, 0);
        }
    }
}
int main(void)
{
interactive_shell();
return (0);
}
