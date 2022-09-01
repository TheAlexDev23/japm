#include <curl/curl.h>

#include "db.h"
#include "japml.h"
#include "package.h"

japml_package_t* japml_get_package_from_local_db(japml_handle_t* handle, char* package_name)
{ 
    // TODO: Open local db file

    // TODO: Use sqlite3 to get package information from local db

    // TODO: Return NULL if no package found

    // TODO: Return package found
}

japml_package_t* japml_get_package_from_remote_db(japml_handle_t* handle, char* package_name)
{
    // TODO: Use CURL to download json file from remote

    // TODO: Use json parser to parse the json file

    // TODO: Return the japml_package_t package from json
}