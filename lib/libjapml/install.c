#include <stdio.h>

#include "japml.h"
#include "log.h"
#include "list.h"
#include "install.h"
#include "package.h"
#include "db.h"
#include "handle.h"
#include "system.h" // japml_run_instructions
#include "japmlcurses.h"

int japml_install_packages(japml_handle_t* handle, japml_list_t* packages)
{
    japml_list_t* it = packages;
    sprintf(handle->log_message, "%i", japml_list_length(packages));
    japml_log(handle, Information, handle->log_message);

    japml_ncurses_pb_set_lim(handle, handle->ncurses_pb_lim + japml_list_length(packages));

    while (it)
    {
        japml_package_t* pkg = (japml_package_t*)it->data;

        if (japml_install_single_package(handle, pkg))
        {
            japml_throw_error(handle, install_error, "Error installing package");
            return 1;
        }

        japml_ncurses_pl_add(handle, pkg, japml_package_install);
        japml_ncurses_pb_add(handle, 1);

        it = japml_list_next(it);
    }

    return 0;
}

int japml_install_single_package(japml_handle_t* handle, japml_package_t* package)
{
    japml_package_t* local_package = NULL;
    if ((local_package = japml_get_package_from_local_db(handle, package->name)) != NULL)
    {
        sprintf(handle->log_message, "Package %s is already installed. Run update if you want to reinstall it", package->name);
        japml_log(handle, Information, handle->log_message);
        free(local_package);
        return 0;
    }

    if (japml_pre_install(handle, package))
    {
        japml_throw_error(handle, install_error, "Error during pre-install");
        return 1;
    }

    sprintf(handle->log_message, "Installing package %s", package->name);
    japml_log(handle, Information, handle->log_message);

    if (japml_run_instructions(package->install))
    {
        japml_throw_error(handle, install_error, "Error running install instructions");
        return 1;
    }

    if (japml_post_install(handle, package))
    {
        japml_throw_error(handle, install_error, "Error during post-install");
        return 1;
    }

    sprintf(handle->log_message, "Package %s installed succesfully", package->name);
    japml_log(handle, Information, handle->log_message);

    // TODO: call progress bar

    // TODO: update package_list ncurses window

    return 0;
}

int japml_pre_install(japml_handle_t* handle, japml_package_t* package)
{
    // * Run pre install script
    japml_list_t* it = package->pre_install;

    sprintf(handle->log_message, "Running pre-install script for %s", package->name);
    japml_log(handle, Debug, handle->log_message);

    while (it)
    {
        if (system((char*)it->data) == -1)
        {
            sprintf(handle->log_message, "Command failed in pre-install script for %s", package->name);
            japml_throw_error(handle, install_error, handle->log_message);
            return 1;
        }
        it = japml_list_next(it);
    }
    return 0;
}

/* 
* Essentially will mark all the packages that package depends on in each's used_by file
*/
void japml_mark_depending_packages(japml_handle_t* handle, japml_package_t* package)
{
    japml_list_t* dep = package->deps;
    while (dep)
    {
        // Sicne it will not append if it's already in used_by there's no need to check that here
        japml_append_depenending_package(handle, package, (japml_package_t*)(dep->data));
        dep = japml_list_next(dep);
    }
}

int japml_post_install(japml_handle_t* handle, japml_package_t* package)
{
    // * Execute post install script
    japml_list_t* it = package->post_install;

    sprintf(handle->log_message, "Running post install script for %s", package->name);
    japml_log(handle, Debug, handle->log_message);
    
    while (it)
    {
        if (system((char*)it->data) == -1)
        {
            sprintf(handle->log_message, "Command failed in post-install script for %s", package->name);
            japml_throw_error(handle, install_error, handle->log_message);
            return 1;
        }

        it = japml_list_next(it);
    }

    japml_log(handle, Information, "Updating local db...");
    japml_add_package_to_local_db(handle, package);
    japml_mark_depending_packages(handle, package);
    japml_log(handle, Information, "Done");

    return 0;
}