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
        japml_package_t* package = japml_get_package_from_remote_db(handle, (char*)(targets->data));
        japml_list_add(handle, &packages, package);
        targets = japml_list_next(targets);
    }

    if (japml_action_create(handle, packages, JAPML_ACTION_TYPE_INSTALL) ||
        japml_action_check(handle) ||
        japml_action_commit(handle))
    {
        japml_free_package_list(packages);
        return -1;
    }

    japml_free_package_list(packages);

    return 0;
}