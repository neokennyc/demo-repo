#include "shell.h"

char *get_history_file(env_var_node_t *env_vars)
{
    char *home_dir = _getenv(env_vars, "HOME");
    if (!home_dir) {
        return NULL;
    }

    char *history_file = malloc(strlen(home_dir) + strlen(HISTORY_FILE) + 2);
    if (!history_file) {
        return NULL;
    }

    strcpy(history_file, home_dir);
    strcat(history_file, "/");
    strcat(history_file, HISTORY_FILE);

    return history_file;
}


/* main function to test the get history file  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int main(void)
{
    // populate a linked list of environment variables
    env_var_node_t *env_vars = populate_env_vars();

    // get the history file using the linked list of environment variables
    char *history_file = get_history_file(env_vars);
    if (history_file == NULL) {
        printf("Error: history file could not be found\n");
    } else {
        printf("History file: %s\n", history_file);
        free(history_file);
    }

    // free the linked list of environment variables
    free_env_var_list(env_vars);

    return 0;
}






/* Old method history features */

typedef struct history_node_s {
    char *command;
    struct history_node_s *next;
} history_node_t;

typedef struct {
    int count;
    history_node_t *head;
} history_t;


// Add a command to the history list
void add_command_to_history(char *command, history_t *history) {
    history_node_t *new_node = malloc(sizeof(history_node_t));
    if (!new_node) {
        fprintf(stderr, "Error: unable to allocate memory for new history node\n");
        return;
    }
    new_node->command = strdup(command);
    new_node->next = history->head;
    history->head = new_node;
    history->count++;
}

// Print the history list
void print_history(history_t *history) {
    history_node_t *current = history->head;
    int i = 0;
    while (current) {
        printf("%d ", i);
        if (current->command) {
            printf("%s\n", current->command);
        } else {
            printf("(nil)\n");
        }
        current = current->next;
        i++;
    }
}


// Free the memory used by the history list
void free_history(history_t *history) {
    history_node_t *current = history->head;
    while (current) {
        history_node_t *temp = current;
        current = current->next;
        free(temp->command);
        free(temp);
    }
    history->count = 0;
    history->head = NULL;
}


/* new method history features*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct history_node_s {
    char *command;
    struct history_node_s *next;
} history_node_t;

typedef struct {
    int count;
    history_node_t *head;
    char *history_file;
} history_t;

// Add a command to the history list
void add_command_to_history(char *command, history_t *history) {
    history_node_t *new_node = malloc(sizeof(history_node_t));
    if (!new_node) {
        fprintf(stderr, "Error: unable to allocate memory for new history node\n");
        return;
    }
    new_node->command = strdup(command);
    new_node->next = history->head;
    history->head = new_node;
    history->count++;
    
    // Write the new command to the history file
    if (history->history_file) {
        FILE *file = fopen(history->history_file, "a");
        if (!file) {
            fprintf(stderr, "Error: unable to open history file for writing\n");
            return;
        }
        fprintf(file, "%s\n", command);
        fclose(file);
    }
}

// Read the history file and populate the history list
void read_history_file(history_t *history) {
    FILE *file = fopen(history->history_file, "r");
    if (!file) {
        fprintf(stderr, "Error: unable to open history file for reading\n");
        return;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1) {
        line[read-1] = '\0'; // Remove the newline character
        add_command_to_history(line, history);
    }
    free(line);
    fclose(file);
}

// Print the history list
void print_history(history_t *history) {
    history_node_t *current = history->head;
    int i = 0;
    while (current) {
        printf("%d ", i);
        if (current->command) {
            printf("%s\n", current->command);
        } else {
            printf("(nil)\n");
        }
        current = current->next;
        i++;
    }
}

// Free the memory used by the history list
void free_history(history_t *history) {
    history_node_t *current = history->head;
    while (current) {
        history_node_t *temp = current;
        current = current->next;
        free(temp->command);
        free(temp);
    }
    history->count = 0;
    history->head = NULL;
}

int main() {
    history_t history = {0};
    history.history_file = "HISTORY_FILE";
    
    read_history_file(&history);
    
    char command[1024];
    while (1) {
        printf("> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        if (command[strlen(command)-1] == '\n') {
            command[strlen(command)-1] = '\0'; // Remove the newline character
        }
        if (strcmp(command, "history") == 0) {
            print_history(&history);
        } else {
            add_command_to_history(command, &history);
        }
    }
    
    free_history(&history);
    
    return 0;
}








#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "history.h"

#define MAX_COMMAND_LENGTH 1024
#define HISTORY_FILE ".myshell_history"

// Add a command to the history list
void add_command_to_history(char *command, history_t *history) {
    history_node_t *new_node = malloc(sizeof(history_node_t));
    if (!new_node) {
        fprintf(stderr, "Error: unable to allocate memory for new history node\n");
        return;
    }
    new_node->command = strdup(command);
    new_node->next = history->head;
    history->head = new_node;
    history->count++;
}

// Read commands from history file and add them to the history list
void read_history_file(history_t *history) {
    char history_file_path[MAX_COMMAND_LENGTH];
    snprintf(history_file_path, MAX_COMMAND_LENGTH, "%s/%s", getenv("HOME"), HISTORY_FILE);

    FILE *fp = fopen(history_file_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open history file %s\n", history_file_path);
        return;
    }

    char buffer[MAX_COMMAND_LENGTH];
    while (fgets(buffer, MAX_COMMAND_LENGTH, fp)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0'; // remove trailing newline
        }
        add_command_to_history(buffer, history);
    }

    fclose(fp);
}

// Free the memory used by the history list
void free_history(history_t *history) {
    history_node_t *current = history->head;
    while (current) {
        history_node_t *temp = current;
        current = current->next;
        free(temp->command);
        free(temp);
    }
    history->count = 0;
    history->head = NULL;
}



/**
 * write_history - writes the current history list to a file
 * @history: pointer to the history list
 */
void write_history(history_t *history)
{
    char *home_dir = getenv("HOME");
    if (!home_dir) {
        fprintf(stderr, "Error: unable to get home directory\n");
        return;
    }

    char *history_file_path = malloc(strlen(home_dir) + strlen(HISTORY_FILE) + 2);
    if (!history_file_path) {
        fprintf(stderr, "Error: unable to allocate memory for history file path\n");
        return;
    }

    sprintf(history_file_path, "%s/%s", home_dir, HISTORY_FILE);
    FILE *fp = fopen(history_file_path, "w");
    if (!fp) {
        fprintf(stderr, "Error: unable to open history file for writing\n");
        free(history_file_path);
        return;
    }

    history_node_t *current = history->head;
    while (current) {
        if (current->command) {
            fprintf(fp, "%s\n", current->command);
        }
        current = current->next;
    }

    fclose(fp);
    free(history_file_path);
}






