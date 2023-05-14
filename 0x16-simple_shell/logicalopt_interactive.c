#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int execute_noninteractive_command(char *command) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return -1;
    } else if (pid == 0) {
        // Child process
        char *args[] = {"/bin/sh", "-c", command, NULL};
        execve("/bin/sh", args, NULL);
        perror("Exec failed");
        _exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            return exit_status;
        } else {
            printf("Child process terminated abnormally.\n");
            return -1;
        }
    }
}

int execute_interactive_command(char *command) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return -1;
    } else if (pid == 0) {
        // Child process
        char *args[] = {"/bin/sh", NULL};
        execve("/bin/sh", args, NULL);
        perror("Exec failed");
        _exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            return exit_status;
        } else {
            printf("Child process terminated abnormally.\n");
            return -1;
        }
    }
}

int main(int argc, char *argv[]) {
    //if (argc < 2) {
     //   printf("Usage: %s <command>\n", argv[0]);
       // return 1;
  //  }

    // Combine command arguments into a single string
    int i;
    char command[256] = "";
    for (i = 1; i < argc; i++) {
        strcat(command, argv[i]);
        strcat(command, " ");
    }

    int exit_status;

  //  if (!(isatty(fileno(stdin)))) {
  if (argc == 1){
        // Interactive mode
        exit_status = execute_interactive_command(command);
    } else {
        // Non-interactive mode
        exit_status = execute_noninteractive_command(command);
    }

/*    printf("Exit status: %d\n", exit_status);*/

    return 0;
}

