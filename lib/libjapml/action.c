#include <stdio.h>

#include "japml.h"
#include "action.h"
#include "package.h"
#include "error.h"
#include "log.h"

int japml_action_create(japml_handle_t* handle, japml_list_t* package_list, japml_action_type_t action_type)
{
    japml_action_t* action = malloc(sizeof(japml_action_t));
    if (!action)
    {
        japml_throw_error(handle, malloc_error, NULL);
    }

    japml_log(handle, Debug, "JAPML: Creating new action");

    action->targets     = package_list;
    action->action_type = action_type;
    action->status      = JAPML_ACTION_STATUS_INITIALIZED;

    japml_log(handle, Debug, "JAPML: Created new action");
    handle->action = action;
    return 0;
}

void japml_action_commit(japml_handle_t* handle)
{
    // TODO: Check if action is valid

    // TODO: If installing get package from remote db and call japml_install_packages

    // TODO: If removing get package from local db and call japml_remove_packages
}