#include <limits.h>
#include <stdio.h>
#include "main.h"                                                       /**                                 * main - Entry point                *                                   * Return: Always 0                  */

int main(void)
{
	int a = 9;
	_printf("%+--+--d\n", 78);
	_printf("%#x", &a);

	return (0);
}
