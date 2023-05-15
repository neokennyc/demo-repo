#include "shell.h"

int _rename(const char *oldname, const char *newname) {
    int oldfile = open(oldname, O_RDONLY);
    if (oldfile == -1) {
        perror("Unable to open the file to be renamed.\n");
        return -1;
    }
    
    int newfile = open(newname, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (newfile == -1) {
        perror("Unable to create the renamed file.\n");
        close(oldfile);
        return -1;
    }
    
    char buffer[1024];
    ssize_t bytesRead;
    
    while ((bytesRead = read(oldfile, buffer, sizeof(buffer))) > 0) {
        if (write(newfile, buffer, bytesRead) != bytesRead) {
            perror("Error occurred while writing to the renamed file.\n");
            close(oldfile);
            close(newfile);
            return -1;
        }
    }
    
    if (bytesRead == -1) {
        perror("Error occurred while reading the file to be renamed.\n");
        close(oldfile);
        close(newfile);
        return -1;
    }
    
    close(oldfile);
    close(newfile);
    return (0);
}
