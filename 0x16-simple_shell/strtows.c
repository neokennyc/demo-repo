#include "shell.h"

/**
 * strtow - splits a string into words. Repeat delimiters are ignored
 * @str: the input string
 * @d: the delimeter string
 * Return: a pointer to an array of strings, else NULL on failure
 */

char **strtow(char *str, char *d)
{
    int i, j, k, numwords = 0;
    char **s;

    if (str == NULL || str[0] == 0)
        return (NULL);
    if (!d)
        d = " ";
    for (i = 0; str[i] != '\0'; i++)
        if (!is_delim(str[i], d) && (is_delim(str[i + 1], d) || !str[i + 1]))
            numwords++;

    if (numwords == 0)
        return (NULL);
    s = malloc((numwords + 1) * sizeof(char *));
    if (!s)
        return (NULL);
    for (i = 0, j = 0; j < numwords; j++)
    {
        while (is_delim(*str, d))
            str++;
        for (k = 0; str[k] != '\0' && !is_delim(str[k], d); k++);
        s[j] = malloc((k + 1) * sizeof(char));
        if (!s[j])
        {
            for (k = 0; k < j; k++)
                free(s[k]);
            free(s);
            return (NULL);
        }
        strncpy(s[j], str, k);
        s[j][k] = '\0';
        str += k;
    }
    s[j] = NULL;
    return (s);
}

