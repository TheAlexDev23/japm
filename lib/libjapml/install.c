#include <stdio.h>
#include <string.h>

#include "japml.h"
#include "log.h"
#include "list.h"
#include "install.h"
#include "package.h"
#include "db.h"
#include "handle.h"
#include "system.h" // japml_run_instructions
#include "japmlcurses.h"
#include "internet.h"
#include "file.h"

int japml_install_packages(japml_handle_t* handle, japml_list_t* packages)
{
    japml_list_t* it = packages;
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
    japml_log(handle, Debug, "Installing single package");

    japml_package_t* local_package = NULL;
    if ((local_package = japml_db_local_get_package(handle, package->name)) != NULL)
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

    char* pkg_dir = japml_get_package_directory(package);
    if (japml_run_instructions_silent( package->install, pkg_dir))
    {
        japml_throw_error(handle, install_error, "Error running install instructions");
        return 1;
    }

    free(pkg_dir);

    if (japml_post_install(handle, package))
    {
        japml_throw_error(handle, install_error, "Error during post-install");
        return 1;
    }

    sprintf(handle->log_message, "Package %s installed succesfully", package->name);
    japml_log(handle, Information, handle->log_message);

    return 0;
}

int japml_pre_install(japml_handle_t* handle, japml_package_t* package)
{
    // * Download files
    japml_list_t* it = package->files;

    while (it)
    {
        japml_package_file_t* file = it->data;
        char* pkg_dir = japml_get_package_directory(package);
        char* file_dir = malloc(strlen(pkg_dir) + strlen(file->rel_file_loc) + 1);
        sprintf(file_dir, "%s%s", pkg_dir, file->rel_file_loc);

        free(pkg_dir);

        if (japml_web_download_file(handle, file->url, file_dir))
        {
            free(file_dir);
            japml_throw_error(handle, custom_error_critical, "Cannot download package file");
            return -1;
        }
        
        free(file_dir);
    
        it = japml_list_next(it);
    }

    sprintf(handle->log_message, "Running pre-install script for %s", package->name);
    japml_log(handle, Debug, handle->log_message);


    char* pkg_dir = japml_get_package_directory(package);
    if (japml_run_instructions_silent( package->pre_install, pkg_dir))
    {
        japml_throw_error(handle, install_error, "Pre install instructions cannot be executed successfully");
        return -1;
    }
    free(pkg_dir);

    return 0;
}

int japml_post_install(japml_handle_t* handle, japml_package_t* package)
{
    sprintf(handle->log_message, "Running post install script for %s", package->name);
    japml_log(handle, Debug, handle->log_message);

    char* pkg_dir = japml_get_package_directory(package);
    if (japml_run_instructions_silent( package->post_install, pkg_dir))
    {
        japml_throw_error(handle, install_error, "Post install instructions cannot be executed successfully");
        return -1;
    }
    free(pkg_dir);

    japml_log(handle, Information, "Updating local db...");
    japml_db_local_add_package(handle, package);
    japml_package_mark_dependencies(handle, package);
    japml_log(handle, Information, "Done");

    return 0;
}