#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

int main(void) {
    // Set environment variables
    _setenv("VAR1", "Value1", 1);
    _setenv("VAR2", "Value2", 1);
    _setenv("VAR3", "Value3", 1);

    // Print environment variables and count
    printf("Printing environment variables:\n");
    size_t count = printenv();
    printf("Total environment variables: %lu\n", count);

    // Unset environment variable
    _unsetenv("VAR2");

    // Print environment variables and count
    printf("Printing environment variables after unset:\n");
    count = printenv();
    printf("Total environment variables: %lu\n", count);

    return 0;
}

