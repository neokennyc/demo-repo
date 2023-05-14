#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void updateAlias(const char* aliasName, const char* filename, const char* newValue) {
    int file = open(filename, O_RDONLY);
    if (file == -1) {
        printf("Error opening file %s\n", filename);
        return;
    }

    int tempFile = open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (tempFile == -1) {
        printf("Error creating temporary file\n");
        close(file);
        return;
    }

    char line[256];
    int aliasExists = 0;
    ssize_t bytesRead;

    while ((bytesRead = read(file, line, sizeof(line))) > 0) {
        char* linePtr = line;
        char* lineEnd = line + bytesRead;

        while (linePtr < lineEnd) {
            char* lineBreak = strchr(linePtr, '\n');
            if (lineBreak == NULL) {
                lineBreak = lineEnd;
            }

            size_t lineLength = lineBreak - linePtr;
            if (strncmp(linePtr, "alias ", 6) == 0) {
                char* aliasStart = linePtr + 6;
                char* equalsSign = strstr(aliasStart, "=");
                if (equalsSign != NULL) {
                    size_t aliasLength = equalsSign - aliasStart;
                    char alias[aliasLength + 1];
                    strncpy(alias, aliasStart, aliasLength);
                    alias[aliasLength] = '\0';

                    if (strcmp(alias, aliasName) == 0) {
                        aliasExists = 1;
                        linePtr = lineBreak + 1;
                        continue; // Skip the existing alias line
                    }
                }
            }
            write(tempFile, linePtr, lineLength);
            write(tempFile, "\n", 1);

            linePtr = lineBreak + 1;
        }
    }

    close(file);
    close(tempFile);

    if (aliasExists) {
        remove(filename); // Remove the original file
        rename("temp.txt", filename); // Rename the temporary file to the original filename
    } else {
        remove("temp.txt"); // Remove the temporary file since no alias was removed
    }

    // Add the new alias entry
    int newAliasFile = open(filename, O_WRONLY | O_APPEND);
    if (newAliasFile == -1) {
        printf("Error opening file %s\n", filename);
        return;
    }

    write(newAliasFile, "alias ", 6);
    write(newAliasFile, aliasName, strlen(aliasName));
    write(newAliasFile, "='", 2);
    write(newAliasFile, newValue, strlen(newValue));
    write(newAliasFile, "'\n", 2);

    close(newAliasFile);
}

