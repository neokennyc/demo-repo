#include <stdio.h>
#include <stdlib.h>                       
#include "main.h"


#define BUFFER_SIZE 1024

int main(void) {
    char *line = NULL;
    size_t line_size = 0;
    ssize_t read;

    printf("Enter some lines of text, followed by an empty line:\n");

    while ((read = _getline(&line, &line_size, stdin)) != -1) 
    {
        printf("Read %ld bytes:\n", read);        printf("%s\n", line);
	break;
	}
    free(line);                                                                         return 0;                             }
