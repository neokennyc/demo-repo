#include <stdio.h>
#include <unistd.h>
#include "main.h"

int writeio(char ch)
{
	static char buffer[BUFF_SIZE];
	static int bufpos = 0;
	if (ch == '\n')
	{
		buffer[bufpos++] = '\n';
		buffer[bufpos] = '\0';
	}

	if (bufpos >= BUFF_SIZE - 1 || ch == '\n') 
	{

	write(1, &buffer, bufpos);
	/*_bufpos += bufpos;*/
		bufpos = 0;
	}
	else if (ch != '\n')
	{
		buffer[bufpos++] = ch;
	}

	return (1);
}
