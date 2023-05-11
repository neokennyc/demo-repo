#include <stdlib.h>
#include <string.h>

char *_getenv(const char* name) 
{

    extern char** environ;
    size_t namelen = strlen(name);
    char** var;

    for (var = environ; *var != NULL; ++var) {
        if (strncmp(name, *var, namelen) == 0 && (*var)[namelen] == '=') {
            return (*var + namelen + 1);
        }
    }

    return NULL;
}

