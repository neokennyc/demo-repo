#include <stdlib.h>
#include <string.h>

extern char **environ;

char *_getenv(const char* name);
int _setenv(const char *name, const char *value, int overwrite);

int _setenv(const char *name, const char *value, int overwrite)
{
    size_t len_name;
    size_t len_value;
    char *new_var;
    int result = 1;

    if (name == NULL || name[0] == '\0' || strchr(name, '=') != NULL) {
        // Invalid name
        return -1;
    }
    if (!overwrite && _getenv(name) != NULL) {
        // Variable already exists and overwrite is not allowed
        return 0;
    }

    // Calculate the lengths of name and value
    len_name = strlen(name);
    len_value = strlen(value);

    // Allocate memory for the new environment variable string
    new_var = malloc(len_name + 1 + len_value + 1);
    if (new_var == NULL) {
        // Failed to allocate memory
        return -1;
    }

    // Construct the new environment variable string
    strcpy(new_var, name);
    strcat(new_var, "=");
    strcat(new_var, value);

    // Find the variable in the environment
    char **envp = environ;
    while (*envp != NULL) {
        if (strncmp(*envp, name, len_name) == 0 && (*envp)[len_name] == '=') {
            // Variable already exists, replace its value
            free(*envp);
            *envp = new_var;
            result = 0;
            break;
        }
        envp++;
    }

    // If the variable does not exist, add it to the environment
    if (*envp == NULL) {
        size_t env_size = envp - environ + 2; // Number of environment variables + NULL
        char **new_env = realloc(environ, env_size * sizeof(char*));
        if (new_env == NULL) {
            // Failed to allocate memory
            free(new_var);
            return -1;
        }
        new_env[env_size - 2] = new_var;
        new_env[env_size - 1] = NULL;
        environ = new_env;
        result = 0;
    }

    return result;
}

