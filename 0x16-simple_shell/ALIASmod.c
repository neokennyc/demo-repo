#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

void removeExtraSpaces(char* str) 
{
    int i, j;
    int len = strlen(str);

    /*Remove leading spaces*/
    while (len > 0 && str[0] == ' ') 
    {
        for (i = 0; i < len; i++) {
            str[i] = str[i + 1];
        }
        len--;
    }

    /*Remove extra spaces*/
    for (i = 0; i < len - 1; i++) 
    {
        if (str[i] == ' ' && str[i + 1] == ' ') {
            for (j = i; j < len; j++) {
                str[j] = str[j + 1];
            }
            len--;
            i--;
        }
    }

    /*Remove trailing spaces */
    while (len > 0 && str[len - 1] == ' ') {
        str[len - 1] = '\0';
        len--;
    }
}





/*Function to load aliases from file*/
void load_aliases();
void retrieve_alias(char* name);
void load_aliases() 
{
	char buffer[BUFFER_SIZE];
	ssize_t bytesRead;
	char line[BUFFER_SIZE];
	int i, fileDescriptor, lineLength = 0;

    fileDescriptor = open(ALIAS_FILE, O_RDONLY);
    if (fileDescriptor == -1) 
    {
        perror("Error opening aliases file");
        return;
    }

    while ((bytesRead = read(fileDescriptor, buffer, BUFFER_SIZE)) > 0) {
        for (i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n') {
                line[lineLength] = '\0'; 
		/* Null-terminate the line*/
                _writef("%s\n", line);
                lineLength = 0;
            } else {
                line[lineLength] = buffer[i];
                lineLength++;
            }
        }
    }
close(fileDescriptor);
}

void update_alias(char *name, char *value)
{
    FILE* file = fopen(ALIAS_FILE, "r");
    if (file == NULL) {
        perror("Error opening aliases file");
        return;
    }

    // Create a temporary file to write the updated aliases
    FILE* temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

    char line[MAX_ALIAS_NAME + MAX_ALIAS_VALUE + 12];  // +12 for "alias name='value'\n"
    int updated = 0; // Flag to indicate if the alias has been updated

    while (fgets(line, sizeof(line), file) != NULL) {
        char alias_name[MAX_ALIAS_NAME];
        char alias_value[MAX_ALIAS_VALUE];
        if (sscanf(line, "alias %[^=]=\'%[^\']\'", alias_name, alias_value) == 2) {
            if (_strcmp(alias_name, name) == 0) {
                // Alias with the same name found, update the value
                fprintf(temp, "alias %s='%s'\n", name, value);
                updated = 1;
            } else {
                // Write the existing alias to the temporary file
                fprintf(temp, "%s", line);
            }
        } else {
            // Write any non-alias lines to the temporary file
            fprintf(temp, "%s", line);
        }
    }

    // If the alias was not found, add it as a new entry
    if (!updated) {
        fprintf(temp, "alias %s='%s'\n", name, value);
    }

    fclose(file);
    fclose(temp);

    // Rename the temporary file to the original file
    if (rename("temp.txt", ALIAS_FILE) != 0) {
        perror("Error renaming temporary file");
        return;
    }
}


// Function to save an alias to file
void save_alias(const char* name, const char* value) 
{
    int fileDescriptor;
    char *filename = ALIAS_FILE;  
    char buffer[1024];
    int size = 0;
    
    /*Create the formatted string manually*/
    size += write(1, "alias ", 6);
    size += write(1, name, strlen(name));
    size += write(1, "='", 2);
    size += write(1, value, strlen(value));
    size += write(1, "'\n", 2);

    /*Open the file in append mode*/
    fileDescriptor = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fileDescriptor < 0) {
        _writef("Error opening the file.\n");
        return;
    }
    
    /*Write the formatted string to the file*/
    write(fileDescriptor, "alias ", 6);
    write(fileDescriptor, name, strlen(name));
    write(fileDescriptor, "='", 2);
    write(fileDescriptor, value, strlen(value));
    write(fileDescriptor, "'\n", 2);

    /*Close the file*/
    close(fileDescriptor);

/*printf("Formatted string appended to the file successfully.\n");*/

}

// Function to retrieve and display an alias
void retrieve_alias(char *name)
{
	char alias_name[MAX_ALIAS_NAME];
	char alias_value[MAX_ALIAS_VALUE];
	char *equalsSign, *quoteEnd, *quoteStart;
	char buffer[BUFFER_SIZE];
	ssize_t bytesRead;
	char line[BUFFER_SIZE];
	int i, fileDescriptor, lineLength = 0;
	fileDescriptor = open(ALIAS_FILE, O_RDONLY);
	if (fileDescriptor == -1)
	{
		perror("Error opening aliases file");
		return;
	}

while ((bytesRead = read(fileDescriptor, buffer, BUFFER_SIZE)) > 0)
{
	for (i = 0; i < bytesRead; i++) 
	{ 
	if (buffer[i] == '\n') {
		line[lineLength] = '\0'; 
		/* Null-terminate the line*/ 
		if (_strstr(line, "alias") != NULL) 
		{
        equalsSign = _strchr(line, '=');
            if (equalsSign != NULL) {
	quoteStart = _strchr(line, '\'');
        quoteEnd = _strrchr(line, '\'');

                if (quoteStart != NULL && quoteEnd != NULL && quoteEnd > quoteStart) {
                    _strncpy(alias_name, line + 6, _strlen(name));
		alias_name[_strlen(name)] = '\0';
                    _strncpy(alias_value, quoteStart + 1, quoteEnd - quoteStart - 1);

                    alias_value[quoteEnd - quoteStart - 1] = '\0';

        /* Perform the comparison and write to stdout*/
                    if (_strcmp(alias_name, name) == 0) {
                        _writef("%s='%s'\n", alias_name, alias_value);
                    }
                }
            }
        }
lineLength = 0;
	} else {
		line[lineLength] = buffer[i];
		lineLength++;
	}
	}
}
/*============================*/
close(fileDescriptor);
}

void process_command(const char* command) {
    const int maxArgs = 10;  // Maximum number of arguments
    char* args[maxArgs];     // Array to store argument pointers
    int numArgs = 0;         // Number of arguments

    // Tokenize the command and store arguments in the array
    char* token = strtok((char*)command, " ");
    while (token != NULL && numArgs < maxArgs) {
        args[numArgs] = _strdup(token);
        numArgs++;
        token = strtok(NULL, " ");
    }

    // Check if the command starts with "alias" and has at least one argument
    if (numArgs >= 2 && _strcmp(args[0], "alias") == 0) {
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


int convertStringToArray(char* inputString
, char*** commandArray) {
    char *delimiter = " \t\n";
    char *keyword = "alias";        
char* token = strtok(inputString, delimiter); 
if (_strncmp(token, keyword, _strlen(keyword)) != 0) {
        return 0;  
	// Input string does not start with the keyword "alias" 
	}
                                              int count = 0;
    while ((token = strtok(NULL, delimiter)) != NULL) {
	    int length = _strlen(keyword) + _strlen(token) + 1;
(*commandArray)[count] = malloc(length + 1);  // Allocate memory for the command 
snprintf((*commandArray)[count], length + 1, "%s %s", keyword, token);              count++;                              }                                     
    return count;                         }
void handlemultiReg(char **command)
{
	int i = 0;
	int equals = 1;
	while(*command)
	{
if (_strncmp(*command, "alias ", 6) == 0 && _strlen(*command) > 6 && equals == 0) {
         //   char name[MAX_ALIAS_NAME];
          //  if (s  f(*command, "alias %s", name) == 1) {
               process_command(*command);
       //     } else {
            //    _writef("Invalid command format. Usage: alias name\n");
          //  }
        } else if (_strncmp(*command, "alias ", 6) == 0 && _strlen(*command) > 7) {
            char name[MAX_ALIAS_NAME];
            char value[MAX_ALIAS_VALUE];
            if (sscanf(*command, "alias %[^=]=\'%[^\']\'", name, value) == 2) {
                update_alias(name, value); // Update the alias
            } else {
                _writef("Invalid alias format. Use: alias name='value'\n");
            }
        } else if (_strcmp(*command, "exit") == 0) {
            break;
        } else {
            _writef("Unknown command: %s\n", *command);
        }

command++;
	}
}

int main() {
    char command[MAX_ALIAS_NAME + MAX_ALIAS_VALUE + 12];  // +12 for "alias name='value'\n"

    char alias_name[78];
    char alias_value[78];
    char *equalsSign, *quoteEnd, *quoteStart;
    while (1) {
        _writef("$ ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';  // Remove trailing newline character

        int n, i = 0;
        int equals = 0;
	char** commandArray = malloc(sizeof(char*) * _strlen(command));
	
        while (command[i]) {
            if (command[i] == '=') {
                equals = equals +  1;
            }
            i++;
        }
//handle multi registrstion of alias
if (equals >= 2)
{
n = convertStringToArray(command, &commandArray);
handlemultiReg(commandArray);
}
else
{
	removeExtraSpaces(command);
        if (_strcmp(command, "alias") == 0) {
            load_aliases();
        } else if (_strncmp(command, "alias ", 6) == 0 && _strlen(command) > 6 && equals == 0) {
          //  char name[MAX_ALIAS_NAME];
          //  if (s f(command, "alias %s", name) == 1) {
		    process_command(command);
     //       } 
//	    else{
//	_writef("Invalid command format. Usage: alias name\n");
  //        }
        } else if (_strncmp(command, "alias ", 6) == 0 && _strlen(command) > 7) 
	{
          //  if (sscanf(command, "alias %[^=]=\'%[^\']\'", name, value) == 2) {
	    if (strstr(command, "alias") != NULL)
	  {
	equalsSign = strchr(command, '=');
	if (equalsSign != NULL) 
	{
	quoteStart = strchr(command, '\'');
	quoteEnd = strrchr(command, '\''); 
	if (quoteStart != NULL && quoteEnd != NULL && quoteEnd > quoteStart)
	{
	strncpy(alias_name, command + 6, equalsSign - (command + 6));
	alias_name[equalsSign - (command + 6)] = '\0';
	strncpy(alias_value, quoteStart + 1, quoteEnd - quoteStart - 1); 
	alias_value[quoteEnd - quoteStart - 1] = '\0';
	update_alias(alias_name, alias_value); // Update the alias
	}
	}
            } else {
                _writef("Invalid alias format. Use: alias name='value'\n");
            }
        } else if (_strcmp(command, "exit") == 0) {
            break;
        } else {
            _writef("Unknown command: %s\n", command);
        }
}
free(commandArray);
    }

    return 0;
}
