#include "main.h"

const char* (*flagtype(char ch))(const char *c, char b)
{
	isflag flag_funcs[] = {
                {'+', isPlus},
		/*{' ', isSpace},*/
		{'#', isHash}};
	int keys = 2;
	int j;

	for (j= 0; j < keys; j++)
		if (flag_funcs[j].flag == ch)
			return (flag_funcs[j].flagtype);
	return (NULL);
}
