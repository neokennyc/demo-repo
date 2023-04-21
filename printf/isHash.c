#include "main.h"
#include <string.h>

const char * isHash(const char *c, char sp)
{
	while (*c == '#')
	{
		c++;
	}
	
	/* Add a prefix for octal and hexadecimal numbers*/
	if (sp == 'o') 
	{
		writeout('0');
	}
	else if (sp == 'x') 
	{
		writeout('0');
		writeout('x');
	} 
	else if  (sp == 'x')
	{
		writeout('0');
		writeout('X');
	}
return (c);
}

