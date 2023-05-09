#include <stdio.h>

#include "japml.h"
#include "log.h"
#include "list.h"
#include "install.h"
#include "package.h"
#include "db.h"
#include "handle.h"

void japml_print_package_list(japml_handle_t* handle, japml_list_t* packages)
{
    while (packages)
    {
        sprintf(handle->log_message, "  %s", ((japml_package_t*)packages->data)->name);
        japml_log(handle, Information, handle->log_message);
        packages = japml_list_next(packages);
    }
}

int japml_run_instructions(japml_list_t* instructions)
{
    while (instructions)
    {
        char *cmd = (char*)instructions->data;
        if (system(cmd) == -1)
        {
            return 1;
        }

        instructions = japml_list_next(instructions);
    }
}

int japml_install_packages(japml_handle_t* handle, japml_list_t* packages)
{
    if (!packages)
    {
        return 1;
    }

    japml_list_t* it = packages;

    while (it)
    {
        japml_package_t* pkg = (japml_package_t*)it->data;

        if (japml_install_single_package(handle, pkg))
        {
            japml_throw_error(handle, install_error, "Error installing package");
            return 1;
        }

        it = japml_list_next(it);
    }

    return 0;
}

int japml_install_single_package(japml_handle_t* handle, japml_package_t* package)
{
    if (japml_get_package_from_local_db(handle, package->name))
    {
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

    if (!it) { return 0; }

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
        japml_append_depenending_packages(handle, package, dep);
        dep = japml_list_next(dep);
    }
}

int japml_post_install(japml_handle_t* handle, japml_package_t* package)
{
    // * Execute post install script
    japml_list_t* it = package->post_install;

    if (it) { return 0; }
    
    sptrinf(handle->log_message, "Running post install script for %s", package->name);
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

    japml_mark_depending_packages(handle, package);

    return 0;
}