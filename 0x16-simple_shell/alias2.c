#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_ALIAS_SIZE 100
#define MAX_LINE_SIZE 200

// Function to display the value of a specific alias
void displayAliasValue(const char *aliasName) {
    FILE *file = fopen("aliases.txt", "r");
    if (file == NULL) {
        printf("No aliases found.\n");
        return;
    }

    char line[MAX_LINE_SIZE];
    while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
        // Check if the line starts with the given alias name
        if (strncmp(line, aliasName, strlen(aliasName)) == 0) {
            // Extract the value from the line
            char *valueStart = strchr(line, '\'');
            if (valueStart != NULL) {
                valueStart++;
                char *valueEnd = strchr(valueStart, '\'');
                if (valueEnd != NULL) {
                    *valueEnd = '\0';  // Null-terminate the value
                    printf("%s='%s'\n", aliasName, valueStart);
                    fclose(file);
                    return;
                }
            }
        }
    }

    printf("Alias '%s' not found.\n", aliasName);
    fclose(file);
}

// Function to add/update an alias in the file
void addAlias(const char *alias) {
    FILE *file = fopen("aliases.txt", "a+");
    if (file == NULL) {
        printf("Error creating/reading the aliases file.\n");
        return;
    }

    // Separate the alias name and value using the equal sign
    char *equalSign = strchr(alias, '=');
    if (equalSign == NULL) {
        printf("Invalid alias format: %s\n", alias);
        fclose(file);
        return;
    }

    // Extract the alias name and value
    char aliasName[MAX_ALIAS_SIZE];
    char aliasValue[MAX_ALIAS_SIZE];
    strncpy(aliasName, alias + 6, equalSign - alias - 7);
    aliasName[equalSign - alias - 7] = '\0';
    strcpy(aliasValue, equalSign + 2, strlen(alias) - (equalSign - alias + 2) - 1);

    // Check if the alias already exists in the file
    char line[MAX_LINE_SIZE];
    int aliasExists = 0;
    long filePos = ftell(file);
    while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
        if (strncmp(line, aliasName, strlen(aliasName)) == 0) {
            aliasExists = 1;
            break;
        }
        filePos = ftell(file);
    }

    if (aliasExists) {
        // Alias already exists, remove the line from the file
        fseek(file, filePos, SEEK_SET);
        ftruncate(fileno(file), filePos);
    }

    // Append the alias to the file
    fseek(file, 0, SEEK_END);
    fprintf(file, "alias %s='%s'\n", aliasName, aliasValue);

    fclose(file);
}

// Function to display all aliases from the file
void displayAliases() {
    FILE *file = fopen("aliases.txt", "r");
    if (file == NULL) {
        printf("No aliases found.\n");
        return;
    }

    char line[MAX_LINE_SIZE];
    while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
}

// Function to handle the "alias" command
// Function to handle the "alias" command
void handleAliasCommand(const char *command) {
    if (strcmp(command, "alias") == 0) {
        displayAliases();
    } else {
        // Split the command by spaces to extract the aliases
        char *token = strtok((char *)command, " ");
        while (token != NULL) {
            // Check if the token starts with "alias"
            if (strncmp(token, "alias", 5) == 0) {
                // Extract the alias name and value
                char *aliasName = strchr(token, ' ');
                if (aliasName != NULL) {
                    aliasName++;
                    char *equalSign = strchr(aliasName, '=');
                    if (equalSign != NULL) {
                        *equalSign = '\0';
                        char *aliasValue = equalSign + 1;
        addAlias(aliasName, aliasValue);
                    } else {
                        printf("Invalid alias format: %s\n", token);
                    }
                } else {
                    printf("Invalid alias format: %s\n", token);
                }
            } else {
                printf("Invalid alias format: %s\n", token);
            }
            
            token = strtok(NULL, " ");
        }
    }
}

// Function to handle the prompt and user input
void handlePrompt() {
    char input[MAX_LINE_SIZE];

    while (1) {
        printf(">> ");
        fgets(input, MAX_LINE_SIZE, stdin);

        // Remove trailing newline character
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        } else if (strncmp(input, "alias ", 6) == 0) {
            // Extract the alias name from the input
            char *aliasName = input + 6;
            displayAliasValue(aliasName);
        } else {
            handleAliasCommand(input);
        }
    }
}

int main() {
    handlePrompt();
    return 0;
}


