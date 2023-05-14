#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdarg.h>

#define ABS(x) ((x) < 0 ? (x) : (-x))

/**
 * struct myOutputs - This struct will be used to filter which print
 * function for i,c,s,d,x,g,f......
 * * @spec: % format specifier to use
 * @selectprint: function pointer to select the correct printer
 */
typedef struct myOutputs
{
	char spec;
	int (*selectprint)(va_list arg);
} OUTS;


int output_alpha(va_list arg);
int output_handle(va_list arg);
int output_char(va_list arg);
int _printf(const char *format, ...);int output_ints(va_list arg);
int (*select_output(char c))(va_list);
int output_unsignedint(va_list arg);

#endif
