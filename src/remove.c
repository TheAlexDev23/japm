#include <stdbool.h>

#include <libjapml/japml.h>
#include <libjapml/handle.h>
#include <libjapml/action.h>
#include <libjapml/list.h>
#include <libjapml/db.h>

#include "remove.h"

int remove_packages(japml_handle_t* handle, japml_list_t* targets, bool recursive)
{
    japml_list_t* packages = NULL;
    while (targets)
    {
        japml_package_t* package = japml_get_package_from_local_db(handle, (char*)(targets->data));
        if (!package)
        {
            sprintf(handle->log_message, "Cannot find package %s in local database.", (char*)(targets->data));
            japml_throw_error(handle, package_not_found_error, handle->log_message);
            return -1;
        }

        japml_list_add(handle, &packages, package);
        targets = japml_list_next(targets);
    }

    if (japml_action_create(handle, packages, recursive ? JAPML_ACTION_TYPE_REMOVE_RECURSIVE : JAPML_ACTION_TYPE_REMOVE) ||
        japml_action_check(handle) ||
        japml_action_commit(handle))
    {
        japml_free_package_list(packages);
        return -1;
    }

    japml_free_package_list(packages);
    return 0;
}