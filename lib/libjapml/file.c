#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "file.h"

int japml_create_file_recursive(char* pathname)
{
    // Copying since pathname might be readonly
    char *path = malloc(sizeof(char) * strlen(pathname));
    strcpy(path, pathname);

    for (char *p = path; (p = strchr(p ,'/')) != NULL; ++p) {
        char c = p[1];
        p[1] = '\0';
        errno = 0;
        if (mkdir(path, 0700) != 0 && errno != EEXIST) {
            return 1;
        }
        p[1] = c;
    }

    int fd = creat(pathname, 0600);
    free(path);

    return fd;
}

void japml_copy_file(char* source, char* dest)
{
    FILE *f = fopen(dest, "w");
    FILE *tmp = fopen(source, "r");

    if(!(f && tmp))
    {
        return;
    }

    char a;
    while ((a = fgetc(tmp)) != EOF )
    {
        fputc(a, f);
    }

    fclose(tmp);
    fclose(f);
}