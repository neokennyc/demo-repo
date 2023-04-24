#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdarg.h>

#define ABS(x) ((x) < 0 ? (x) : (-x))

#define BUFF_SIZE 1024
/**
 * struct myOutputs - This struct will be used to filter which print
 * function for i,c,s,d,x,g,f......
 * * @spec: % format specifier to use
 * @selectprint: function pointer to select the correct printer
 */
typedef struct myOutputs
{
	char spec;
	int (*selectprint)(va_list arg);
} OUTS;

/**
 * struct myflags - This struct will be used to determine the flags
 * function for + Space #.....
 * * @flag: # + ' '
 * @flagtype: function pointer to
 select the correct flag functiom
 */
typedef struct myflags
{
	char flag;
       const  char* (*flagtype)(const char *ch, char, va_list);

} isflag;


const char * isZerostr(const char * c, char sp, char *);
const char * isZeroint(const char * c, char sp, int);
const char * isPlus(const char * c, char, va_list);
const char * isHash(const char * c, char, va_list);
const char * isSpace(const char * c, char, va_list);
const char * isZerocustom(const char * c, char, va_list);
const char * isZeroflot(const char * c, char sp, double arg);
int _putchar(char c);
int writeout(char ch);
int output_alpha(va_list arg);
int output_handle(va_list arg);
int output_hexlower(va_list arg);
int output_hexupper(va_list arg);
int output_ptraddress(va_list arg);
int output_char(va_list arg);
int output_bits(va_list arg);
int output_r(va_list arg);
int output_ROT13(va_list arg);
int output_octal(va_list arg);
int _printf(const char *format, ...);int output_ints(va_list arg);
int (*select_output(char c))(va_list);
const char* (*flagtype(char c))(const char *, char, va_list);
int output_unsignedint(va_list arg);

#endif
