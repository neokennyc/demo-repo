#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

char *_getenv(const char* name);
int _setenv(const char *name, const char *value, int overwrite);

void change_directory(const char *path) {
    char new_dir[MAX_COMMAND_LENGTH];

    if (path[0] == '/') {
        // Absolute path
        strncpy(new_dir, path, sizeof(new_dir));
    } else {
        // Relative path
        char current_dir[MAX_COMMAND_LENGTH];
        if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
            perror("getcwd");
            return;
        }
        snprintf(new_dir, sizeof(new_dir), "%s/%s", current_dir, path);
    }

    if (chdir(new_dir) == -1) {
        perror("chdir");
        return;
    }

    char resolved_dir[MAX_COMMAND_LENGTH];
    if (realpath(new_dir, resolved_dir) == NULL) {
        perror("realpath");
        return;
    }

    // Update PWD and OLDPWD environment variables
    setenv("OLDPWD", getenv("PWD"), 1);
    setenv("PWD", resolved_dir, 1);
    printf("Changed directory to: %s\n", resolved_dir);
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

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        printf("$ ");
        fgets(command, sizeof(command), stdin);

        if (strcmp(command, "exit\n") == 0) {
            break;
        }

        if (execute_command(command) == 0) {
            printf("Invalid command\n");
        }
    }

    return 0;
}

