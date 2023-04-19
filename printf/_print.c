#include <stdio.h>
#include <stdarg.h>
#include "main.h"
/**
 * _printf - Printf function
 * @format: format.
 * Return: Printed chars.
 */

int _printf(const char *format, ...)
{
	int printed_chars;
	va_list args;
	char buffer[12];
	int num, i, len;

	va_start(args, format);
num = 0;
	printed_chars = 0;
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'c':
                    {
                        char c = va_arg(args, int);
                        putchar(c);
                        printed_chars++;
                        break;
                    }
                case 's':
                    {
                        char *s = va_arg(args, char *);
                        if (s == NULL) {
                            fprintf(stderr, "error\n");
                            va_end(args);
                            return -1;
                        }
                        while (*s != '\0') {
                            putchar(*s);
                            s++;
                            printed_chars++;
                        }
                        break;
                    }
                case '%':
                    {
                        putchar('%');
                        printed_chars++;
                        break;
                    }
		case 'd':
		    {
			    num = 0;
num = va_arg(args, int);
len = 0;
if (num < 0) 
{
	putchar('-');
	printed_chars++;
	num = -num;
}
do 
{
	buffer[len++] = num % 10 + '0';
	num /= 10;
} while (num != 0);
for (i = len - 1; i >= 0; i--) 
{
	putchar(buffer[i]);
	printed_chars++;
}
break;
}
                default:
                    {
                        fprintf(stderr, "error: invalid format specifier '%c' in _printf\n", *format);
                        va_end(args);
                        return -1;
                    }
            }
        } else {
            putchar(*format);
            printed_chars++;
        }
        format++;
    }
    
    va_end(args);
    putchar('\n');
    return printed_chars;
}



