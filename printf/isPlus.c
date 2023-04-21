#include "main.h"

const char * isPlus(const char * c, char sp)
{
	int n;
if (sp == 'd' || sp == 'i')
{
	while (*c == '+' || *c == '-')
	{
if (*c == '+')
	n = n * 1;
if (*c == '-')
	n = n * -1;

		c++;
	}
	if (n < 0)
		writeout('-');
	else
		writeout('+');
}
return (c);
}


