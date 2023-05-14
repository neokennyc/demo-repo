#include <stdlib.h>
#include <string.h>
#include "shell.h"

/**
 * add_node_end - Adds a node to the end of a linked list
 * @head: Double pointer to the head node
 * @str: String to store in the new node
 *
 * Return: Address of the new node, or NULL on failure
 */
list_t *add_node_end(list_t **head, char *str)
{
    list_t *new_node = malloc(sizeof(list_t));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->str = strdup(str);
    new_node->num = 0;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        list_t *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }

    return new_node;
}

/**
 * populate_env_list - Populates a linked list with the environment variables
 * @head: Double pointer to the head node
 *
 * Return: 0 on success, -1 on failure
 */
int populate_env_list(list_t **head)
{
    size_t i;

    for (i = 0; environ[i]; i++) {
        if (add_node_end(head, environ[i]) == NULL) {
            return -1;
        }
    }

    return 0;
}

/**
 * free_env_list - Frees a linked list and its nodes
 * @head: Pointer to the head node
 */
void free_env_list(list_t *head)
{
    list_t *current = head;
    while (current != NULL) {
        list_t *next = current->next;
        free(current->str);
        free(current);
        current = next;
    }
}


