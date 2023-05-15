#include <libjapml/japml.h>
#include <libjapml/list.h>
#include <libjapml/db.h>
#include <libjapml/package.h>

#include "search.h"

int search_packages(japml_handle_t* handle, japml_list_t* targets)
{
    japml_list_t* packages = NULL;
    while (targets)
    {
        japml_package_t* package = japml_db_remote_get_package(handle, (char*)(targets->data));
        if (!package)
        {
            return -1;
        }

        japml_package_print_details(handle, package);
        targets = japml_list_next(targets);
    }

    return 0;
}