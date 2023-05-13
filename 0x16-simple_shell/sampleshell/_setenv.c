#include <stdlib.h>
#include <string.h>

char *_getenv(const char* name);
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

    // Allocate memory for the new environment variable string
    len_name = strlen(name);
    len_value = strlen(value);
    new_var = malloc(len_name + 1 + len_value + 1);
    if (new_var == NULL) {
        // Failed to allocate memory
        return -1;
    }

    // Construct the new environment variable string
    strcpy(new_var, name);
    strcat(new_var, "=");
    strcat(new_var, value);

    // Set the environment variable
    result = putenv(new_var);

    // Free the allocated memory
    free(new_var);

    return result;
}
	
