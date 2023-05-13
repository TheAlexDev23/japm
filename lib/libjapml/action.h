#ifndef _JAPML_ACTION_H_INCLUDED
#define _JAPML_ACTION_H_INCLUDED

#include <stdbool.h>

#include "japml.h"
#include "list.h"
#include "handle.h"

enum _japml_action_type
{
    JAPML_ACTION_TYPE_INSTALL,
    JAPML_ACTION_TYPE_REMOVE,
    JAPML_ACTION_TYPE_REMOVE_RECURSIVE
};

enum _japml_action_status
{
    JAPML_ACTION_STATUS_INITIALIZED,
    JAPML_ACTION_STATUS_APPROVED,
    JAPML_ACTION_STATUS_COMITED,
    JAPML_ACTION_STATUS_ABORTED
};

struct _japml_action
{
    /* A list of japml_package_t */
    japml_list_t* targets;

    japml_action_type_t action_type;
    japml_action_status_t status;
};

/* Creates an action and initializes it with the values provided */
int japml_action_create(japml_handle_t* handle, japml_list_t* package_list, japml_action_type_t action_type);

/* Checks the action, making sure no dependency break happens. Also adds dependencies in case of install */
int japml_action_check(japml_handle_t* handle);
/* Performs the action*/
int japml_action_commit(japml_handle_t* handle);

#endif