#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LENGTH 1024
#define MAX_ARGS 64

int execute_command(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("hsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Forking error
        perror("hsh");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int run_shell_interactive() {
    char *line;
    char **args;
    int status;

    do {
        printf("$ ");
        line = NULL;
        size_t line_buf_size = 0;
        getline(&line, &line_buf_size, stdin);
        if (strcmp(line, "\n") == 0) {
            continue;
        }

        // Handle comments
        char *comment_start = strchr(line, '#');
        if (comment_start != NULL) {
            *comment_start = '\0';
        }

        args = malloc(MAX_ARGS * sizeof(char *));
        char *token;
        int arg_count = 0;

        token = strtok(line, " \t\n");
        while (token != NULL) {
            args[arg_count] = token;
            arg_count++;
            token = strtok(NULL, " \t\n");
        }
        args[arg_count] = NULL;

        status = execute_command(args);

        free(line);
        free(args);
    } while (status);

    return 0;
} 
int run_shell_noninteractive(char *filename, char **command_args, int num_args) {
    FILE *file = NULL;
    char *line = NULL;
    char **args = NULL;
    int status = 0;

    // Check if filename is provided
    if (filename != NULL) {
        file = fopen(filename, "r");
        if (!file) {
            perror(filename);
            return 1;
        }
    }

    do {
        line = NULL;
        size_t line_buf_size = 0;
        ssize_t line_length = -1;

        // Read from file or command line arguments
        if (file) {
            line_length = getline(&line, &line_buf_size, file);
        } else if (num_args > 0) {
            // Combine the command line arguments into a single string
            size_t args_len = 0;
            for (int i = 0; i < num_args; i++) {
                args_len += strlen(command_args[i]) + 1; // +1 for space separator
            }
            line = (char *) malloc(args_len + 1); // +1 for null terminator
            line[0] = '\0';
            for (int i = 0; i < num_args; i++) {
                strcat(line, command_args[i]);
                if (i != num_args - 1) {
                    strcat(line, " ");
                }
            }
            line_length = strlen(line);
        } else {
            // No input provided
            break;
        }

        // Remove newline character from the end of the line
        if (line_length > 0 && line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
            line_length--;
        }

        // Handle comments
        char *comment_start = strchr(line, '#');
        if (comment_start != NULL) {
            *comment_start = '\0';
            line_length = comment_start - line;
        }

        if (line_length == 0) {
            // Empty line or line with only comments
            continue;
        }

        // Split the line into arguments
        args = (char **) malloc(MAX_ARGS * sizeof(char *));
        char *token = strtok(line, " \t\n");
        int arg_count = 0;
        while (token != NULL) {
            args[arg_count] = token;
            arg_count++;
            token = strtok(NULL, " \t\n");
        }
        args[arg_count] = NULL;

        status = execute_command(args);

        free(line);
        free(args);
    } while (file || num_args > 0);

    if (file) {
        fclose(file);
    }

    return 0;
}


int main(int argc, char **argv) {
    int is_interactive = isatty(STDIN_FILENO);
    int exit_code = 0;

    if (argc == 2) {
        // Non-interactive mode with a file
        exit_code = run_shell_noninteractive(argv[1], NULL, 0);
    } else if (argc > 2) {
        // Non-interactive mode with command line arguments
        exit_code = run_shell_noninteractive(NULL, &argv[1], argc - 1);
    } else if (is_interactive) {
        // Interactive mode
        exit_code = run_shell_interactive();
    } else {
        // No input provided
        fprintf(stderr, "Error: No input provided.\n");
        exit_code = 1;
    }

    return exit_code;
}

