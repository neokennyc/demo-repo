#include <stdlib.h>
#include <string.h>
#include "shell.h"

int _setenv(const char *name, const char *value, int overwrite) {
    if (name == NULL || *name == '\0' || strchr(name, '=') != NULL) {
        return -1;
    }

    struct env_var *cur_var = env_list;
    struct env_var *prev_var = NULL;

    // Search for the existing variable with the same name
    while (cur_var != NULL) {
        if (strcmp(cur_var->name, name) == 0) {
            if (!overwrite) {
                return 0;
            } else {
                // Free the existing value and set the new value
                free(cur_var->value);
                cur_var->value = strdup(value);
                return 0;
            }
        }
        prev_var = cur_var;
        cur_var = cur_var->next;
    }

    // Variable not found, create a new one
    struct env_var *new_var = malloc(sizeof(struct env_var));
    if (new_var == NULL) {
        return -1;
    }
    new_var->name = strdup(name);
    new_var->value = strdup(value);
    new_var->next = NULL;

    // Add the new variable to the list
    if (prev_var == NULL) {
        env_list = new_var;
    } else {
        prev_var->next = new_var;
    }

    return 0;
}

char *_getenv(const char *name) {
    if (name == NULL || *name == '\0' || strchr(name, '=') != NULL) {
        return NULL;
    }

    struct env_var *cur_var = env_list;
    while (cur_var != NULL) {
        if (strcmp(cur_var->name, name) == 0) {
            return cur_var->value;
        }
        cur_var = cur_var->next;
    }

    return NULL;
}

int _unsetenv(const char *name) {
    if (name == NULL || *name == '\0' || strchr(name, '=') != NULL) {
        return -1;
    }

    struct env_var *cur_var = env_list;
    struct env_var *prev_var = NULL;

    // Search for the variable with the given name
    while (cur_var != NULL) {
        if (strcmp(cur_var->name, name) == 0) {
            // Remove the variable from the list
            if (prev_var == NULL) {
                env_list = cur_var->next;
            } else {
                prev_var->next = cur_var->next;
            }
            free(cur_var->name);
            free(cur_var->value);
            free(cur_var);
            return 0;
        }
        prev_var = cur_var;
        cur_var = cur_var->next;
    }

    return 0;
}


size_t printenv(void) 
{
    struct env_var *cur_var = env_list;
    size_t count = 0;
    while (cur_var != NULL) {
        printf("%s\n", cur_var->value);
        cur_var = cur_var->next;
        count++;
    }
    return count;
}


