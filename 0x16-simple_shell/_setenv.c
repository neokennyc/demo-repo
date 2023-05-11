#include <stdlib.h>
#include <string.h>

int _setenv(const char *name, const char *value, int overwrite) {
    extern char **environ;
	if (name == NULL || name[0] == '\0' || strchr(name, '=') != NULL) {
        // Invalid name
        return -1;
    }

    // Calculate the required size for the new environment list
    int num_vars;
    for (num_vars = 0; environ[num_vars] != NULL; num_vars++);

    size_t len_name = strlen(name);
    size_t len_value = strlen(value);

    // Check if the variable already exists
       int existing_var_index = -1;
    for (int i = 0; i < num_vars; i++) {
        if (strncmp(environ[i], name, len_name) == 0 && environ[i][len_name] == '=') {
            existing_var_index = i;
            break;
        }
    }

    if (existing_var_index != -1) {
        // Variable exists
        if (!overwrite) {
            // Overwrite is not allowed
            return 0;
        } else {
            // Overwrite the existing value
            char *new_var = malloc(len_name + len_value + 2);  // +2 for '=' and '\0'
            if (new_var == NULL) {
                // Failed to allocate memory
                return -1;
            }

            // Construct the new environment variable string
            snprintf(new_var, len_name + len_value + 2, "%s=%s", name, value);

            // Replace the existing variable with the new value
            free(environ[existing_var_index]);
            environ[existing_var_index] = new_var;

            return 0;
        }
    } else {
        // Variable doesn't exist, add it to the environment list

        // Allocate memory for the new environment variable string
        char *new_var = malloc(len_name + len_value + 2);  // +2 for '=' and '\0'
        if (new_var == NULL) {
            // Failed to allocate memory
            return -1;
        }

        // Construct the new environment variable string
        snprintf(new_var, len_name + len_value + 2, "%s=%s", name, value);

        // Allocate memory for the new environment list
        char **new_environ = malloc((num_vars + 2) * sizeof(char *));
        if (new_environ == NULL) {
            // Failed to allocate memory
            free(new_var);
            return -1;
        }

        // Copy existing variables to the new environment list
        for (int i = 0; i < num_vars; i++) {
            new_environ[i] = environ[i];
        }

        // Add the new variable to the end of the new environment list
        new_environ[num_vars] = new_var;

        // Set the last element of the new environment list to NULL
        new_environ[num_vars + 1] = NULL;

        // Replace the environment list with the new list
        environ = new_environ;

        return 0;
    }
}

