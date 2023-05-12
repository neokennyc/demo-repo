#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALIAS_NAME 50
#define MAX_ALIAS_VALUE 100
#define ALIAS_FILE "aliases.txt"

// Function to load aliases from file
void load_aliases() {
    FILE* file = fopen(ALIAS_FILE, "r");
    if (file == NULL) {
        perror("Error opening aliases file");
        return;
    }

    char line[MAX_ALIAS_NAME + MAX_ALIAS_VALUE + 12];  // +12 for "alias name='value'\n"
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);  // Display output to stdout
    }

    fclose(file);
}

// Function to save an alias to file
void save_alias(const char* name, const char* value) {
    FILE* file = fopen(ALIAS_FILE, "a");
    if (file == NULL) {
        perror("Error opening aliases file");
        return;
    }

    fprintf(file, "alias %s='%s'\n", name, value);

    fclose(file);
}

// Function to retrieve and display an alias
void retrieve_alias(const char* name) {
    FILE* file = fopen(ALIAS_FILE, "r");
    if (file == NULL) {
        perror("Error opening aliases file");
        return;
    }

    char line[MAX_ALIAS_NAME + MAX_ALIAS_VALUE + 12];  // +12 for "alias name='value'\n"
    while (fgets(line, sizeof(line), file) != NULL) {
        char alias_name[MAX_ALIAS_NAME];
        char alias_value[MAX_ALIAS_VALUE];
        if (sscanf(line, "alias %[^=]=\'%[^\']\'", alias_name, alias_value) == 2) {
            if (strcmp(alias_name, name) == 0) {
                printf("%s='%s'\n", alias_name, alias_value);
            }
        }
    }

    fclose(file);
}

// Function to check if an alias exists
int is_alias_exists(const char* name) {
    FILE* file = fopen(ALIAS_FILE, "r");
    if (file == NULL) {
        perror("Error opening aliases file");
        return 0;
    }

    char line[MAX_ALIAS_NAME + MAX_ALIAS_VALUE + 12];  // +12 for "alias name='value'\n"
    while (fgets(line, sizeof(line), file) != NULL) {
        char alias_name[MAX_ALIAS_NAME];
        if (sscanf(line, "alias %[^=]", alias_name) == 1) {
            if (strcmp(alias_name, name) == 0) {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

int main() {
    char command[MAX_ALIAS_NAME + MAX_ALIAS_VALUE + 12];  // +12 for "alias name='value'\n"

    while (1) {
        printf("Enter a command (type 'exit' to quit): ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';  // Remove trailing newline character

        if (strcmp(command, "alias") == 0) {
            load_aliases();
        } else if (strncmp(command, "alias ", 6) == 0 && strlen(command) > 6) {
            char name[MAX_ALIAS_NAME];
            if (sscanf(command, "alias %s", name) == 1) {
                retrieve_alias(name);
            } else {
            char* token = strtok(command + 6, " ");
	    while (token != NULL) {
                retrieve_alias(token);
                token = strtok(NULL, " ");
	    }}
        } else if ((strncmp(command, "alias ", 6) == 0) && strlen(command) > 7) {
            char name[MAX_ALIAS_NAME];
            char value[MAX_ALIAS_VALUE];
            if (sscanf(command, "alias %[^=]=\'%[^\']\'", name, value) == 2) {
                if (!is_alias_exists(name)) {
                    save_alias(name, value);
                } else {
                    printf("Alias already exists: %s\n", name);
                }
            } else {
                printf("Invalid alias format. Use: alias name='value'\n");
            }
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Unknown command: %s\n", command);
        }
    }

    return 0;
}

