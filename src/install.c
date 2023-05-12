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

    if (japml_action_create(handle, packages, JAPML_ACTION_TYPE_INSTALL))
    {
        return -1;
    }
    
    if (japml_action_check(handle))
    {
        return -1;
    }

    if (japml_action_commit(handle))
    {
        return -1;
    }

    return 0;
}