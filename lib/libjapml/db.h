#ifndef _DB_H_INCLUDED
#define _DB_H_INCLUDED

#include "japml.h"

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

#endif