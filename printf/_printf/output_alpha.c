#include "main.h"

int output_alpha(va_list arg)
{
	char *str = va_arg(arg, char *);

	/*(void)f; */
	int count_alpha;

	count_alpha = 0;
	if (!str)
	{
		str = "(null)";
			return (0);
	}

	while (*str != '\0') 
	{
putchar(*str);
str++;		
count_alpha++;
	}
	return (count_alpha);
}
