#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 1024

// Reads a single character from the given stream and returns it.
// Returns EOF if there is an error or if the end of the stream is reached.
static int read_char(FILE *stream) {
    int c = fgetc(stream);
    if (c == EOF && ferror(stream)) {
        return EOF;
    }
    return c;
}

// Resizes the buffer to the given size and returns a pointer to the new buffer.
// Returns NULL if there is an error.
static char *resize_buffer(char *buffer, size_t current_size, size_t new_size) {
    char *new_buffer = realloc(buffer, new_size);
    if (new_buffer == NULL) {
        free(buffer);
        return NULL;
    }
    memset(new_buffer + current_size, 0, new_size - current_size);
    return new_buffer;
}

// Reads a line of text from the given stream and stores it in the buffer.
// The buffer will be dynamically resized as needed.
// Returns the number of bytes read, or -1 if there is an error.
ssize_t _getline(char **lineptr, size_t *n, FILE *stream) {
    static char *buffer = NULL;
    static size_t buffer_size = 0;
    static size_t buffer_pos = 0;

    ssize_t read = 0;

    if (lineptr == NULL || n == NULL) {
        return -1;
    }

    if (buffer == NULL) {
        buffer = malloc(INITIAL_BUFFER_SIZE);
        if (buffer == NULL) {
            return -1;
        }
        buffer_size = INITIAL_BUFFER_SIZE;
        buffer_pos = 0;
    }

    while (1) {
        // Check if we need to resize the buffer
        if (buffer_pos + 1 >= buffer_size) {
            size_t new_size = buffer_size * 2;
            char *new_buffer = resize_buffer(buffer, buffer_size, new_size);
            if (new_buffer == NULL) {
                return -1;
            }
            buffer = new_buffer;
            buffer_size = new_size;
        }

        // Read the next character
        int c = read_char(stream);
        if (c == EOF) {
            if (read == 0) {
                return -1;
            } else {
                break;
            }
        }

        // Store the character in the buffer
        buffer[buffer_pos++] = (char) c;
        ++read;

        // Check if we have reached the end of the line
        if (c == '\n') {
            break;
        }
    }

    // Null-terminate the buffer
    buffer[buffer_pos] = '\0';

    // Return the line and its length
    *lineptr = buffer;
    *n = buffer_pos;

    // Reset the buffer position for the next call
    buffer_pos = 0;

    return read;
}

