#ifndef _FILE_H_INCLUDED
#define _FILE_H_INCLUDED

/* 
* Creates file at pathname and parent directories if they dont exist.
* Will override pathname if it already exists.
*/
int japml_create_file_recursive(char* pathname);

/* Copies contents of file source to file dest */
void japml_copy_file(char* source, char* dest);

#endif