#ifndef _JAPML_DB_H_INCLUDED
#define _JAPML_DB_H_INCLUDED

#include "japml.h"
#include "handle.h"

// If othe parameters except for url will not be needed, remove this
struct _japml_db_remote
{
	char *url;
};

void japml_create_local_db(japml_handle_t* handle);

#define MAX_CHAR_LIST_LENGTH 5000

japml_list_t* japml_string_to_list(japml_handle_t* handle, char* string_list);
char* japml_list_to_string(japml_handle_t* handle, japml_list_t* list);

japml_package_t* japml_get_package_from_local_db(japml_handle_t* handle, char* package_name);
japml_package_t* japml_get_package_from_remote_db(japml_handle_t* handle, char* package_name);

int japml_add_package_to_local_db(japml_handle_t* handle, japml_package_t* package);
int japml_remove_package_from_local_db(japml_handle_t* handle, japml_package_t* package);

#endif