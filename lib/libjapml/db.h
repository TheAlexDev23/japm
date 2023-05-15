#ifndef _JAPML_DB_H_INCLUDED
#define _JAPML_DB_H_INCLUDED

#include "japml.h"
#include "handle.h"

// TODO: Consider removing this and rather make handle.remote_dbs a list of strings
struct _japml_db_remote
{
	char *url;
};

void japml_create_local_db(japml_handle_t* handle);

#define MAX_CHAR_LIST_LENGTH 5000

/* Throws an error and mentions the sql error */
void japml_db_error(japml_handle_t* handle);

japml_list_t* japml_db_local_get_all_packages(japml_handle_t* handle);

/* Allocates and returns a japml_package_t* fetched from the local database */
japml_package_t* japml_db_local_get_package(japml_handle_t* handle, char* package_name);
/* Allocates and returns a japml_package_t* fetched from the remote database */
japml_package_t* japml_db_remote_get_package(japml_handle_t* handle, char* package_name);

int japml_db_local_add_package(japml_handle_t* handle, japml_package_t* package);
int japml_db_local_remove_package(japml_handle_t* handle, japml_package_t* package);

#endif