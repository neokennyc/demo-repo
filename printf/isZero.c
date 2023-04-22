#include "main.h"
#include <ctype.h>

const char * isZero(const char * c, char sp, va_list arg)
{
	int len, i = 0, k, tmp, value, n, num_digits, zeros_to_print;
        int j = i + 1;
	int arr[49];
	int spaces_to_print;
        int zero_padding = 0;
        int left_justify = 0;
        int width = 0;

	/* lets go one step back*/
c--;
	len = strlen(c);
        while (j < len && !isalpha(c[j]))
        {
            if (c[j] == '0')
                zero_padding = 1;
            else if (c[j] == '-')
                left_justify = 1;
            else if (isdigit(c[j]))
                width = width * 10 + c[j] - '0';
            j++;
        }
	c+=j;
	c++;

      /*  char specifier = c[j];*/
        if (sp == 'd' || sp == 'i')
        {
		value = va_arg(arg, int);
            n = 0;
            if (value == 0) 
	    {
                writeout('0');
                n = 1;
            } else 
	    {
                if (value < 0) {
                    writeout('-');
                    value = -value;
                    n++;
                }
		num_digits = 0;
		tmp = value;
                while (tmp > 0) {
                    num_digits++;
                    tmp /= 10;
                }
                n += num_digits;
                if (zero_padding && width > n) 
		{
			zeros_to_print = width - n;
                    for (k = 0; k < zeros_to_print; k++) {
                        writeout('0');
                    }
                }
             /*   int arr[num_digits];*/
                for (k = 0; k < num_digits; k++) {
                    arr[k] = value % 10;
                    value /= 10;
                }
                for (k = num_digits - 1; k >= 0; k--) {
                    writeout(arr[k] + '0');
                }
                if (left_justify && width > n) {
                    spaces_to_print = width - n;
                    for (k = 0; k < spaces_to_print; k++) {
                        writeout(' ');
                    }
                }
            }
        }

/* d / i is taken care of */

return (c);
}

