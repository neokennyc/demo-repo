int set_env_var(const char *name, const char *value)
{
    /* Set the environment variable using exec */
    char *argv[] = {"export", name, value, NULL};
    char *envp[] = {NULL};
    execve("/usr/bin/export", argv, envp);

    /* If execve() returns, there was an error */
    perror("execve");
    return -1;
}
int remove_env_var(env_var_node_t **head, const char *name)
{
    env_var_node_t *prev = NULL, *curr = *head;
    int removed = 0;

    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            if (prev == NULL) {
                /* Head node needs to be removed */
                *head = curr->next;
            } else {
                prev->next = curr->next;
            }

            /* Remove the environment variable using exec */
            char *argv[] = {"unset", name, NULL};
            char *envp[] = {NULL};
            execve("/usr/bin/unset", argv, envp);

            /* Free the memory used by the node */
            free(curr->name);
            free(curr->value);
            free(curr);

            /* Set the removed flag and exit the loop */
            removed = 1;
            break;
        }

        prev = curr;
        curr = curr->next;
    }

    return removed;
}

/* save the command in a script script.sh*/
#!/bin/bash

echo "export MY_VAR=my_value" >> ~/.bashrc

/*run the command this way in c code */
system("/path/to/script.sh");


/*or this way In c code as well*/
char *argv[] = {"/path/to/script.sh", NULL};
char *envp[] = {NULL};
execve("/bin/bash", argv, envp);

