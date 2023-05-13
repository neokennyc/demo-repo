#include <stdarg.h>
#include <string.h>
#include <unistd.h>  /*for write */
#include "shell.h"
void _writef(const char *format, ...) {
    // Set up the argument list
    va_list args;
    va_start(args, format);

    // Iterate over the format string
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            if (*format == 's') {
                // Fetch the next argument as a string
                char *str = va_arg(args, char *);

                // Get the length of the string
                size_t len = _strlen(str);

                // Write the string to the standard output
                write(STDOUT_FILENO, str, len);
            }
        } else {
            // Write regular characters to the standard output
            write(STDOUT_FILENO, format, 1);
        }
        format++;
    }

    // Clean up the argument list
    va_end(args);
}

