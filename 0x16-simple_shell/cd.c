#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

extern char **environ;  // Declare the external environment variable list

char *_getenv(const char* name);
int _setenv(const char *name, const char *value, int overwrite);

void change_directory(const char *path) {
    char current_dir[MAX_COMMAND_LENGTH];
    char new_dir[MAX_COMMAND_LENGTH];

    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd");
        return;
    }

    if (chdir(path) == -1){
        perror("chdir");
        return;
    }

    if (getcwd(new_dir, sizeof(new_dir)) == NULL) {
        perror("getcwd");
        return;
    }

    // Update PWD and OLDPWD environment variables
    _setenv("OLDPWD", current_dir, 1);
    _setenv("PWD", new_dir, 1);
    printf("Changed directory to: %s\n", new_dir);
}

int execute_command(char *command) {
    char *args[MAX_COMMAND_LENGTH];
    char *token;
    int i = 0;

    token = strtok(command, " \t\n");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            change_directory(_getenv("HOME"));
        } else if (strcmp(args[1], "-") == 0) {
            change_directory(_getenv("OLDPWD"));
        } else {
            change_directory(args[1]);
        }
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, NULL, 0);
    }

    return 1;
}

char *_getenv(const char* name) {
    int i;
    size_t len = strlen(name);

    for (i = 0; environ[i] != NULL; i++) {
        if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=') {
            return &environ[i][len+1];
        }
    }

    return NULL;
}

int main()
{
char command[MAX_COMMAND_LENGTH];                                                   while (1) {
        printf("$ ");                             fgets(command, sizeof(command), stdin);                                                                                       if (strcmp(command, "exit\n") == 0) {                                                   break;                                }                                                                                   if (execute_command(command) == 0) {                                                    printf("Invalid command\n");          }                                     }                                                                                   return 0;
	}
