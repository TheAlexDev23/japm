#include <stdio.h>
#include <string.h>

#include "action.h"
#include "japml.h"
#include "log.h"
#include "package.h"
#include "error.h"
#include "db.h"
#include "list.h"

#include "install.h"
#include "remove.h"

#include "japmlcurses.h" // japml_nY_dialogue

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

void japml_action_check_type_remove(japml_handle_t* handle)
{
    japml_list_t* it = handle->action->targets;
    // Iterate through all targets checking if they have pacakages depending on them
    while (it)
    {
        japml_package_t* pkg = (japml_package_t*)(it->data);
        japml_get_depending_packages(handle, pkg);

        if (pkg->depending_packages)
        {
            handle->action->status = JAPML_ACTION_STATUS_ABORTED;
            sprintf(handle->log_message, "Removing package breaks dependency: %s", ((japml_package_t*)(pkg->depending_packages->data))->name);
            japml_throw_error(handle, dependency_break_error, handle->log_message);
        }

        it = japml_list_next(it);
    }
}

void japml_action_check_type_remove_recursive(japml_handle_t* handle)
{
restart_type_removal_recursive:
    japml_list_t* it = handle->action->targets;
    // Iterate through all targets checking if they have pacakages depending on them
    while (it)
    {
        japml_package_t* pkg = (japml_package_t*)(it->data);
        japml_get_depending_packages(handle, pkg);

        japml_list_t* depending_packages = pkg->depending_packages;
        while(depending_packages)
        {
            // In case a new dependant package needs to be removed, we need to rerun in case it has unremoved dependant_packages
            if (!japml_add_package_to_list_no_repeat(handle, handle->action->targets, (japml_package_t*)(depending_packages->data)))
            {
                goto restart_type_removal_recursive;
            }

            depending_packages = japml_list_next(depending_packages);
        }

        it = japml_list_next(it);
    }
}

void japml_action_check_type_install(japml_handle_t* handle)
{
restart_type_install:
    japml_list_t* it = handle->action->targets;
    // Iterate through all targets checking if they have pacakages depending on them
    while (it)
    {
        japml_package_t* pkg = (japml_package_t*)(it->data);
        japml_get_depending_packages(handle, pkg);

        japml_list_t* dependencies = pkg->deps;
        while(dependencies)
        {
            // In a case a new dependency needs to be added to install list we will need to rerun in case it also has uninstalled dependencies
            if (!japml_add_package_to_list_no_repeat(handle, handle->action->targets, (japml_package_t*)(dependencies->data)))
            {
                goto restart_type_install;
            }

            dependencies = japml_list_next(dependencies);
        }

        it = japml_list_next(it);
    }
}

int japml_action_check(japml_handle_t* handle)
{
    switch (handle->action->action_type)
    {
        case JAPML_ACTION_TYPE_REMOVE:
            japml_action_check_type_remove(handle);
            break;
        case JAPML_ACTION_TYPE_REMOVE_RECURSIVE:
            japml_action_check_type_remove_recursive(handle);
            break;
        case JAPML_ACTION_TYPE_INSTALL:
            japml_action_check_type_install(handle);
            break;
    }

    if (handle->action->action_type == JAPML_ACTION_TYPE_INSTALL)
    {
        japml_log(handle, Information, "The following packages will be installed:");
    }
    else
    {
        japml_log(handle, Information, "The following packages will be removed:");
    }

    japml_list_t* it = handle->action->targets;
    while (it)
    {
        japml_log(handle, Information, ((japml_package_t*)it->data)->name);
        it = japml_list_next(it);
    }

    if (japml_ncurses_Yn_dialogue(handle, "Do you wish to continue? "))
    {
        handle->action->action_type = JAPML_ACTION_STATUS_APPROVED;
        return 0;
    }
    else
    {
        handle->action->action_type = JAPML_ACTION_STATUS_ABORTED;
        return 1;
    }
}

int japml_action_commit(japml_handle_t* handle)
{
    if (handle->action->status != JAPML_ACTION_STATUS_APPROVED)
    {
        japml_throw_error(handle, action_not_accepted_error, "Action is not approved. Cannot commit");
        return 1;
    }

    if (handle->action->action_type == JAPML_ACTION_TYPE_INSTALL)
    {
        japml_install_packages(handle, handle->action->targets);
    }
    else
    {
        japml_throw_error(handle, custom_error_critical, "japml_action_commit for removal not yet implemented");
    }
}