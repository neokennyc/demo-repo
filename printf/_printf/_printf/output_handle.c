#include "main.h"
#include <stdio.h>

int output_handle(va_list arg)
{
	(void)arg;
	/*8(void)f;*/
	putchar('%');
	return(1);
}
