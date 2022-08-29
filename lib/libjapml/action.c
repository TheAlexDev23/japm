#include "japml.h"
#include "action.h"
#include "error.h"
#include "log.h"

japml_action_t* japml_action_create(japml_handle_t* handle, japml_list_t* package_list, japml_action_type_t action_type)
{
    japml_action_t* action = malloc(sizeof(japml_action_t));
    if (!action)
    {
        _japml_throw_error(handle, malloc_error, NULL);
    }

    action->targets     = package_list;
    action->action_type = action_type;
    action->status      = JAPML_ACTION_STATUS_INITIALIZED;

    return action;
}

void japml_action_commit(japml_handle_t* handle, japml_action_t* action)
{
    japml_log(handle, Debug, "Action commit began");

    
}