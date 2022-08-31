#ifndef _DB_H_INCLUDED
#define _DB_H_INCLUDED

#include "japml.h"
#include "handle.h"

struct _japml_db_local
{
	char *file_location;
};

struct _japml_db_remote
{
	char *url;
};

/* Run a sql command on the local db*/
int japml_local_db_run(japml_handle_t* handle, char* sql_commmand);

japml_package_t* japml_get_package_from_local_db(japml_handle_t* handle, char* package_name);
japml_package_t* japml_get_package_from_remote_db(japml_handle_t* handle, char* package_name);

#endif