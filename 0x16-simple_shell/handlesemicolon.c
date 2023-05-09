#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64

void execute_command(char* command) {
    char* arguments[MAX_ARGUMENTS];
    int num_arguments = 0;
    arguments[num_arguments] = strtok(command, " \t\n");
    while (arguments[num_arguments] != NULL) {
        num_arguments++;
        arguments[num_arguments] = strtok(NULL, " \t\n");
    }
    if (num_arguments == 0) {
        return;
    }
    pid_t pid = fork();
    if (pid == 0) {
        if (execve(arguments[0], arguments, NULL) == -1) {
            perror("execve failed");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else {
        int status;
        if (wait(&status) == -1) {
            perror("wait failed");
            exit(EXIT_FAILURE);
        }
    }
}

void execute_commands(char** commands) {
    int i = 0;
    while (commands[i] != NULL) {
        execute_command(commands[i]);
        i++;
    }
}

void interactive_mode() {
    char user_input[MAX_COMMAND_LENGTH];
    char* commands[MAX_ARGUMENTS];
    int num_commands = 0;
    
    while (1) {
        printf("$ ");
        if (fgets(user_input, MAX_COMMAND_LENGTH, stdin) == NULL) {
            break;
        }
        if (strcmp(user_input, "exit\n") == 0) {
            break;
        }
        num_commands = 0;
        commands[num_commands] = strtok(user_input, ";\n");
        while (commands[num_commands] != NULL) {
            num_commands++;
            commands[num_commands] = strtok(NULL, ";\n");
        }
        execute_commands(commands);
    }
}

void non_interactive_mode(int argc, char* argv[]) {
    char user_input[MAX_COMMAND_LENGTH];
    char* commands[MAX_ARGUMENTS];
    int num_commands = 0;
    FILE* script_file = NULL;
    
    for (int i = 1; i < argc; i++) {
        num_commands = 0;
        commands[num_commands] = strtok(argv[i], " \t\n");
        while (commands[num_commands] != NULL) {
            num_commands++;
            commands[num_commands] = strtok(NULL, " \t\n");
        }
        execute_commands(commands);
    }
    
    for (int i = 1; i < argc; i++) {
        char* command = argv[i];
        char* token = strtok(command, ";");
        while (token != NULL) {
            num_commands = 0;
            commands[num_commands] = strtok(token, " \t\n");
            while (commands[num_commands] != NULL) {
                num_commands++;
                commands[num_commands] = strtok(NULL, " \t\n");
            }
            execute_commands(commands);
            token = strtok(NULL, ";");
        }
    }
    
    script_file = fopen(argv[1], "r");
    if (script_file == NULL) {
        perror("Error opening script file");
        exit(EXIT_FAILURE);
    }
    
    while (fgets(user_input, MAX_COMMAND_LENGTH, script_file) != NULL) {
        char* token = strtok(user_input, ";");
        while (token != NULL) {
            num_commands = 0;
            commands[num_commands] = strtok(token, " \t\n");
            while (commands[num_commands] != NULL) {
                num_commands++;
                commands[num_commands] = strtok(NULL, " \t\n");
            }
            execute_commands(commands);
            token = strtok(NULL, ";");
        }
    }
    fclose(script_file);
}




int main(int argc, char* argv[]) {
    if (argc == 1) {
        interactive_mode();
    } else if (argc == 2) {
        non_interactive_mode(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s [script]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    return 0;
}

