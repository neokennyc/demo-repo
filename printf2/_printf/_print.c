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
	int print_length;
	va_list args;
int (*output)(va_list);

	va_start(args, format);
	print_length = 0;
	if (!format)
		return (-1);



    while (*format != '\0') {
        if (*format == '%') 
	{
            format++;
output = select_output(*format);
if (output)
	print_length += output(args);
        
	}
	else 
	{
        putchar(*format);
           print_length++;
        }
        format++;
    }
	va_end(args);
	/*putchar('\n');*/
	return (print_length);
}
