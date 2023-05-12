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

void process_command(const char* command) {
    const int maxArgs = 10;  // Maximum number of arguments
    char* args[maxArgs];     // Array to store argument pointers
    int numArgs = 0;         // Number of arguments

    // Tokenize the command and store arguments in the array
    char* token = strtok((char*)command, " ");
    while (token != NULL && numArgs < maxArgs) {
        args[numArgs] = strdup(token);
        numArgs++;
        token = strtok(NULL, " ");
    }

    // Check if the command starts with "alias" and has at least one argument
    if (numArgs >= 2 && strcmp(args[0], "alias") == 0) {
        // Pass the arguments one by one to the retrieve_alias() function
        for (int i = 1; i < numArgs; i++) {
            retrieve_alias(args[i]);
        }
    }

    // Free the dynamically allocated argument pointers
    for (int i = 0; i < numArgs; i++) {
        free(args[i]);
    }
}




int main() {
    char command[MAX_ALIAS_NAME + MAX_ALIAS_VALUE + 12];  // +12 for "alias name='value'\n"

    while (1) {
        printf("$ ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';  // Remove trailing newline character
int i = 0;
int equals = 0;
	while(command[i])
{
	if (command[i] == '=')
	{
		equals = 1;
		break;
	}
	i++;
}

        if (strcmp(command, "alias") == 0) {
            load_aliases();
        } else if (strncmp(command, "alias ", 6) == 0 && strlen(command) > 6 && equals == 0) {
            char name[MAX_ALIAS_NAME];
	    char name2[MAX_ALIAS_NAME];
            if (sscanf(command, "alias %s %s", name, name2) >= 1) {
//        retrieve_alias(name);
//	if (name2[0])
//	retrieve_alias(name2);
	process_command(command);
            } else {
                printf("Invalid command format. Usage: alias name\n");
            }
        } else if (strncmp(command, "alias ", 6) == 0 && strlen(command) > 7) {
            char name[MAX_ALIAS_NAME];
            char value[MAX_ALIAS_VALUE];
            if (sscanf(command, "alias %[^=]=\'%[^\']\'", name, value) == 2) {
                save_alias(name, value);
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
