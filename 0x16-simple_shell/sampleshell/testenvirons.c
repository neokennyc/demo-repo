#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

int main(void) {
    // Test mysetenv
    char *test_var2;
    printf("Setting environment variable TEST_VAR1 to \"test\"\n");
    int result = _setenv("TEST_VAR1", "test", 1);
    if (result != 0) {
        fprintf(stderr, "Error setting environment variable\n");
        exit(1);
    }
    
    char *test_var = _getenv("TEST_VAR1");
    if (test_var == NULL) {
        fprintf(stderr, "Error getting environment variable\n");
        exit(1);
    }
    
    printf("TEST_VAR1 = \"%s\"\n", test_var);
    
    printf("Setting environment variable TEST_VAR2 to \"new value\"\n");
    result = _setenv("TEST_VAR2", "PASCAL", 1);
    if (result != 0) {
        fprintf(stderr, "Error setting environment variable\n");
        exit(1);
    }
    
    test_var2 = _getenv("TEST_VAR2");
    if (test_var2 == NULL) {
        fprintf(stderr, "Error getting environment variable\n");
        exit(1);
    }
    
    printf("TEST_VAR2 = \"%s\"\n", test_var2);
    
    // Test myunsetenv
    printf("Unsetting environment variable TEST_VAR\n");
    result = _unsetenv("TEST_VAR2");
    if (result != 0) {
        fprintf(stderr, "Error unsetting environment variable\n");
        exit(1);
    }
    
    test_var = _getenv("TEST_VAR2");
    if (test_var != NULL) {
        fprintf(stderr, "Error: TEST_VAR2 still set to \"%s\"\n", test_var);
        exit(1);
    }
    
    printf("TEST_VAR2 is unset\n");
    
    return 0;
}




