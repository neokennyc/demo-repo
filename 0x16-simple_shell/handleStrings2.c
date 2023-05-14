#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/**
 * _strchr
 * @str: ====
 * @ch: ====
 * Return: =====
 */
char *_strchr(char *str, char ch) {
    while (*str != '\0') {
        if (*str == ch) {
            return ((char*)str);
        }
        str++;
    }

    if (ch == '\0') {
        return (char*)str;
    }

    return (NULL);
}
/**
 * _strtok - =====
 * @str: ======
 * @delimeters: ======
 * Return: =======
 */
char *_strtok(char *str, char *delimiters) {
    static char* token = NULL;
    static char* rest = NULL;
    bool found = false;

    if (str != NULL) {
        rest = str;
    }

    if (rest == NULL || *rest == '\0') {
        return NULL;
    }

    token = rest;
    while (*rest != '\0') {
        if (_strchr(delimiters, *rest) != NULL) {
            *rest = '\0';
            rest++;
            found = true;
            break;
        }
        rest++;
    }

    if (!found) {
        rest = NULL;
    }

    return (token);
}


