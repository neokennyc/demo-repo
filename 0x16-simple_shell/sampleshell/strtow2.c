#include "shell.h"

/**
 * _count_words - helper function to count number of words in string
 * @str: the input string
 * @delim: the delimiter character
 * Return: the number of words in the string
 */
static int _count_words(char *str, char delim)
{
	int count = 0, in_word = 0;

	while (*str)
	{
		if (*str == delim)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}

		str++;
	}

	return count;
}

/**
 * _next_word - helper function to find the start of the next word in a string
 * @str: the input string
 * @delim: the delimiter character
 * Return: a pointer to the start of the next word in the string, or NULL
 */
static char *_next_word(char *str, char delim)
{
	while (*str && *str == delim)
		str++;

	if (!*str)
		return NULL;

	return str;
}

/**
 * _word_length - helper function to find the length of a word in a string
 * @str: the input string
 * @delim: the delimiter character
 * Return: the length of the word
 */
static int _word_length(char *str, char delim)
{
	int len = 0;

	while (*str && *str != delim)
	{
		str++;
		len++;
	}

	return len;
}

/**
 * strtow2 - splits a string into words. Repeat delimiters are ignored
 * @str: the input string
 * @delim: the delimiter string
 * Return: a pointer to an array of strings, else NULL on failure
 */
char **strtow2(char *str, char *delim)
{
	char **words;
	char *word;
	int i, count;

	if (!str || !*str)
		return NULL;

	if (!delim || !*delim)
		delim = " ";

	count = _count_words(str, *delim);
	if (!count)
		return NULL;

	words = malloc((count + 1) * sizeof(char *));
	if (!words)
		return NULL;

	for (i = 0, word = _next_word(str, *delim); i < count; i++)
	{
		int len = _word_length(word, *delim);

		words[i] = malloc(len + 1);
		if (!words[i])
		{
			while (--i >= 0)
				free(words[i]);
			free(words);
			return NULL;
		}

		strncpy(words[i], word, len);
		words[i][len] = '\0';

		word += len;
		word = _next_word(word, *delim);
	}

	words[count] = NULL;

	return words;
}
