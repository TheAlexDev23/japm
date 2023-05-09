#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "file.h"

int japml_create_file_recursive(char* pathname)
{
    for (char *p = pathname; (p = strchr(p ,'/')) != NULL; ++p) {
        char c = p[1];
        p[1] = '\0';
        errno = 0;
        if (mkdir(pathname, 0700) != 0 && errno != EEXIST) {
            return 1;
        }
        p[1] = c;
    }
    int fd = creat(pathname, 0600);

    return fd;
}

void japml_copy_file(char* source, char* dest)
{
    f = fopen(dest, "w");
    tmp = fopen(source, "r");

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
    free(file);
}