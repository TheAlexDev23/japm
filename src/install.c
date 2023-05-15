#include <libjapml/japml.h>

#include <libjapml/action.h>
#include <libjapml/list.h>
#include <libjapml/package.h>
#include <libjapml/db.h>

#include "install.h"

int install_packages(japml_handle_t* handle, japml_list_t* targets)
{
    japml_list_t* packages = NULL;
    while (targets)
    {
        japml_package_t* package = japml_db_remote_get_package(handle, (char*)(targets->data));
        if (!package)
        {
            sprintf(handle->log_message, "Cannot find package %s in remote database.", (char*)(targets->data));
            japml_throw_error(handle, package_not_found_error, handle->log_message);
            return -1;
        }

        japml_list_add(handle, &packages, package);
        targets = japml_list_next(targets);
    }

    if (japml_action_create(handle, packages, JAPML_ACTION_TYPE_INSTALL) ||
        japml_action_check(handle) ||
        japml_action_commit(handle))
    {
        japml_package_free_list(packages);
        return -1;
    }

    japml_package_free_list(packages);

    return 0;
}