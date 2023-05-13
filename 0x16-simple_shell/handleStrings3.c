#include "shell.h"
/**
 * _strstr - -=====
 * @haystack: =======
 * @needle: ====
 * Return: If the substring is located - a pointer to the beginning
 */

char *_strstr(char *haystack, char *needle)
{
	int index;

	if (*needle == 0)
		return (haystack);

	while (*haystack)
	{
		index = 0;

		if (haystack[index] == needle[index])
		{
			do {
				if (needle[index + 1] == '\0')
					return (haystack);

				index++;

			} while (haystack[index] == needle[index]);
		}

		haystack++;
	}

	return (NULL);
}


/**
 * _strncpy - ====
 * @dest: ===
 * @src: ====
 * @n: ====
 * Return: =+=====
 */
char *_strncpy(char *dest, char *src, int n)
{
	int index = 0, src_len = 0;

	while (src[index++])
		src_len++;

	for (index = 0; src[index] && index < n; index++)
		dest[index] = src[index];

	for (index = src_len; index < n; index++)
		dest[index] = '\0';

	return (dest);
}
/**
 * _strrchr - ======
 * @str: ========
 * @character: ========
 * Return: ======
 */
char *_strrchr(char *str, int character) 
{
    char *last_occurrence = NULL;

    while (*str != '\0') {
        if (*str == character)
            last_occurrence = str;
        ++str;
    }

    if (character == '\0')
        return (char*)str;

    return ((char*)last_occurrence);
}

