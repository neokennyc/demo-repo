#include "main.h"
#include <string.h>

const char * isHash(const char *c, char sp)
{
	while (*c == '#')
	{
		c++;
	}
	
	/* Add a prefix for octal and hexadecimal numbers*/

	writeout('0');
	if (sp == 'o') 
	{
		writeout('o');                  
	}
	else if (sp == 'x') 
	{
		writeout('x');
	} 
	else 
	{
		writeout('X');
	}
return (c);
}

