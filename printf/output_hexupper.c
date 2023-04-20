#include <stdio.h>
#include "main.h"

int output_hexupper(va_list arg)
{
    unsigned int uint;
    int i, count;
    char hex_str[20];
    int hex_len;
    int digit;

    uint = va_arg(arg, unsigned int);
    i = 0;
    count = 0;
    hex_len = 0;

    /* Convert the unsigned int argument to an uppercase hexadecimal string */
    do 
    {
        digit = uint % 16;
        if (digit < 10) 
	{
            hex_str[hex_len++] = digit + '0';
        } 
	else 
	{
            hex_str[hex_len++] = 'A' + digit - 10;
        }
        uint /= 16;
    } while (uint != 0);

    /* Print the hexadecimal string in reverse order */
    for (i = hex_len - 1; i >= 0; i--) {
        putchar(hex_str[i]);
        count++;
    }

    return count;
}
