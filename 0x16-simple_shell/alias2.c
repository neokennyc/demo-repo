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

    // Append the alias to the file
    fseek(file, 0, SEEK_END);
    fprintf(file, "%s\n", alias);

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
void handleAliasCommand(const char *command) {
    if (strcmp(command, "alias") == 0) {
        displayAliases();
    } else {
        // Split the command by spaces to extract the aliases
	    char *token = strtok((char *)command, " ");
		
        while (token != NULL) {
            addAlias(token);
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
	FILE *file = fopen("aliases.txt", "a+");
    handlePrompt();
    return 0;
}
