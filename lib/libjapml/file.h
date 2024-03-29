#ifndef _JAPML_FILE_H_INCLUDED
#define _JAPML_FILE_H_INCLUDED

#include "japml.h"
#include "list.h"

struct _japml_package_file
{
    char* url;
    char* rel_file_loc;
};

/* Returns directory of package */
char* japml_get_package_directory(japml_package_t* package);

/* Returns a list of FILE opened with each value of files. By default with write permissions.*/
japml_list_t* japml_create_file_list(japml_list_t* files);

/* 
* Creates file at pathname and parent directories if they dont exist.
* Will override pathname if it already exists.
*/
int japml_file_create_recursive(char* pathname);

/* Copies contents of file source to file dest */
void japml_file_copy(char* source, char* dest);

/* Recursively removes directory at path. Basically a rm -rf system command */
int japml_delete_dir_rf(char *path);

#endif