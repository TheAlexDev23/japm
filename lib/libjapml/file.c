#define _XOPEN_SOURCE 500 // access to ntfw

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <ftw.h>

#include "file.h"
#include "log.h"
#include "package.h"


char* japml_get_package_directory(japml_package_t* package)
{
    char* dir = malloc(strlen("/var/japml/packages//") + strlen(package->name) + 1);
    sprintf(dir, "/var/japml/packages/%s/", package->name);
    return dir;
}

japml_list_t* japml_create_file_list(japml_list_t* files)
{
    japml_list_t* list = NULL;
    while (files)
    {
        FILE *f = fopen((char*)(files->data), "w");
        if (!f)
        {
            continue;
        }

        japml_list_add(NULL, &list, f);
        files = japml_list_next(files);
    }

    return list;
}

int japml_file_create_recursive(char* pathname)
{
    // Copying since pathname might be readonly
    char *path = malloc(strlen(pathname) + 1);
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

void japml_file_copy(char* source, char* dest)
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

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    return rv;
}

int japml_delete_dir_rf(char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}