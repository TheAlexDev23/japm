#ifndef _DB_H_INCLUDED
#define _DB_H_INCLUDED

#include "japml.h"
#include "handle.h"

struct _japml_db_local
{
	japml_handle_t *handle;
	char *file_location;
};

struct _japml_db_remote
{
	japml_handle_t *handle;
	char *url;
};

/* Run a sql command on the local db*/
int japml_local_db_run(japml_handle_t* handle, char* sql_commmand);

#endif