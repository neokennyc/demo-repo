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

