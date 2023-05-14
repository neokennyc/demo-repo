#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct env_var_node {
    char *name;
    char *value;
    struct env_var_node *next;
} env_var_node_t;

/* Global variable for the external environment variables */
extern char **environ;

env_var_node_t *populate_env_vars() {
    env_var_node_t *head = NULL;
    char **envp = environ;
    while (*envp) {
        char *env_str = *envp++;
        char *equals_pos = strchr(env_str, '=');
        if (equals_pos) {
            size_t name_len = equals_pos - env_str;
            char *name = malloc(name_len + 1);
            strncpy(name, env_str, name_len);
            name[name_len] = '\0';
            char *value = equals_pos + 1;
            env_var_node_t *new_node = malloc(sizeof(env_var_node_t));
            new_node->name = name;
            new_node->value = value;
            new_node->next = head;
            head = new_node;
        }
    }
    return head;
}

void print_env_vars(env_var_node_t *head) {
    env_var_node_t *node = head;
    while (node != NULL) {
        printf("%s=%s\n", node->name, node->value);
        node = node->next;
    }
}

char *_getenv(env_var_node_t *head, const char *name) {
    env_var_node_t *node = head;
    while (node != NULL) {
        if (strcmp(node->name, name) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

/* Set or update an environment variable */
int _setenv(env_var_node_t **head, const char *name, const char *value, int overwrite) {
    /* Check if the variable already exists */
    env_var_node_t *node = *head;
    env_var_node_t *prev_node = NULL;
    while (node != NULL) {
        if (strcmp(node->name, name) == 0) {
            /* If the variable exists and overwrite is not allowed, return an error */
            if (!overwrite) {
                return -1;
            }
            /* Otherwise, update the value of the variable */
            free(node->value);
            node->value = strdup(value);
            /* Update the actual environment variable in the shell, if possible */
            char *env_str = malloc(strlen(name) + strlen(value) + 2);
            sprintf(env_str, "%s=%s", name, value);
            int res = putenv(env_str);
            if (res != 0) {
                fprintf(stderr, "Error updating environment variable %s\n", name);
            }
            return 0;
        }
        prev_node = node;
        node = node->next;
    }
    /* If the variable does not exist, create a new node */
    env_var_node_t *new_node = malloc(sizeof(env_var_node_t));
    new_node->name = strdup(name);
    new_node->value = strdup(value);
    new_node->next = NULL;
    /* Add the new node to the list */
    if (prev_node == NULL) {
        *head = new_node;
    } else {
        prev_node->next = new_node;
    }
    /* Update the actual environment variable in the shell, if possible */
    char *env_str = malloc(strlen(name) + strlen(value) + 2);
sprintf(env_str, "%s=%s", name, value);
int res = putenv(env_str);
if (res != 0) {
fprintf(stderr, "Error setting environment variable %s\n", name);
return -1;
}
return 0;
}
/* Remove an environment variable */
int _unsetenv(env_var_node_t **head, const char *name) {
env_var_node_t *node = *head;
env_var_node_t prev_node = NULL;
while (node != NULL) {
if (strcmp(node->name, name) == 0) {
/* Remove the environment variable from the actual shell environment */
int res = unsetenv(name);
if (res != 0) {
if (strcmp(node->name, name) == 0) {
/* Remove the environment variable from the actual shell environment */
int res = unsetenv(name);
if (res != 0) {
fprintf(stderr, "Error removing environment variable %s\n", name);
}
/* Remove the node from the list */
if (prev_node == NULL) {
head = node->next;
} else {
prev_node->next = node->next;
}
free(node->name);
free(node->value);
free(node);
return 0;
}
prev_node = node;
node = node->next;
}
/* If the variable does not exist, return an error */
return -1;
}

/**
 * _unsetenv - Removes an environment variable.
 * @head: A pointer to a pointer to the head of the environment linked list.
 * @name: The name of the variable to be removed.
 *
 * Return: 1 if the variable is removed, 0 otherwise.
 */
int _unsetenv(env_var_node_t **head, const char *name)
{
    env_var_node_t *node = *head;
    int removed = 0;

    while (node != NULL) {
        if (strcmp(node->name, name) == 0) {
            env_var_node_t *next_node = node->next;
            delete_node_env(head, node);
            node = next_node;
            removed = 1;
            remove_env_var(name); // remove from source
        } else {
            node = node->next;
        }
    }

    return removed;
}


/**
 * remove_env_var - Remove an environment variable by name from the linked list
 *                  and the source.
 * @head: A pointer to the head of the linked list.
 * @name: The name of the environment variable to remove.
 *
 * Return: 1 if the variable was removed, 0 otherwise.
 */
int remove_env_var(env_var_node_t **head, const char *name)
{
    env_var_node_t *prev = NULL, *curr = *head;
    int removed = 0;

    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            if (prev == NULL) {
                /* Head node needs to be removed */
                *head = curr->next;
            } else {
                prev->next = curr->next;
            }

            /* Remove the environment variable from the source */
            char *unset_cmd = malloc(strlen(name) + 8);
            if (unset_cmd != NULL) {
                sprintf(unset_cmd, "unset %s", name);
                system(unset_cmd);
                free(unset_cmd);
            }

            /* Free the memory used by the node */
            free(curr->name);
            free(curr->value);
            free(curr);

            /* Set the removed flag and exit the loop */
            removed = 1;
            break;
        }

        prev = curr;
        curr = curr->next;
    }

    return removed;
}








int main(int argc, char argv[]) {
/* Populate the environment variable list */
env_var_node_t *env_vars = populate_env_vars();
/* Print the environment variables */
print_env_vars(env_vars);

/* Set an environment variable */
int res = _setenv(&env_vars, "MY_VAR", "Hello, world!", 1);
if (res == 0) {
    printf("MY_VAR set successfully.\n");
} else {
    printf("Error setting MY_VAR.\n");
}

/* Update an environment variable */
res = _setenv(&env_vars, "MY_VAR", "Hello again, world!", 1);
if (res == 0) {
    printf("MY_VAR updated successfully.\n");
} else {
    printf("Error updating MY_VAR.\n");
}

/* Try to set an environment variable that already exists, without overwriting it */
res = _setenv(&env_vars, "MY_VAR", "Should not be printed", 0);
if (res == 0) {
    printf("Error: should not have been able to set MY_VAR.\n");
} else {
    printf("Could not set MY_VAR again, as expected.\n");
}

/* Remove an environment variable */
res = _unsetenv(&env_vars, "MY_VAR");
if (res == 0) {
    printf("MY_VAR removed successfully.\n");
} else {
    printf("Error removing MY_VAR.\n");
}

/* Try to remove an environment variable that does not exist */
res = _unsetenv(&env_vars, "NONEXISTENT_VAR");
if (res == 0) {
    printf("Error: should not have been able to remove NONEXISTENT_VAR.\n");
} else {
    printf("Could not remove NONEXISTENT_VAR, as expected.\n");
}

/* Print the environment variables again */
print_env_vars(env_vars);

return 0;
} 
