#include <stdio.h>

#include "japml.h"
#include "log.h"
#include "list.h"
#include "install.h"
#include "package.h"
#include "handle.h"

void japml_install_packages(japml_handle_t* handle, japml_list_t* packages)
{
    if (!packages)
    {
        return;
    }

    japml_list_t* it = packages;

    while (it)
    {
        japml_package_t* pkg = (japml_package_t*)it->data;

        japml_install_single_package(handle, pkg);

        it = japml_list_next(it);
    }
}

void japml_install_single_package(japml_handle_t* handle, japml_package_t* package)
{
    japml_pre_install(handle, package);

    sprintf(handle->log_message, "Installing package %s", package->name);
    japml_log(handle, Information, handle->log_message);

    japml_list_t* it = package->install;
    while (it)
    {
        char *cmd = (char*)it->data;
        if (system(cmd) == -1)
        {
            japml_throw_error(handle, install_error, NULL);
        }
    }

    japml_post_install(handle, package);

    sprintf(handle->log_message, "Package %s installed succesfully", package->name);
    japml_log(handle, Information, handle->log_message);

    // TODO: call progress bar callback (not implemented yet)

    // TODO: update package_list ncurses window
}

void japml_pre_install(japml_handle_t* handle, japml_package_t* package)
{
    // * Run pre install script
    japml_list_t* it = package->pre_install;

    if (it)
    {
        sprintf(handle->log_message, "Running pre-install script for %s", package->name);
        japml_log(handle, Debug, handle->log_message);
    }

    while (it)
    {
        if (system((char*)it->data) == -1)
        {
            sprintf(handle->log_message, "Command failed in pre-install script for %s", package->name);
            japml_throw_error(handle, install_error, handle->log_message);
        }
        it = japml_list_next(it);
    }

    // * Install build dependencies

    // Print build deps
    it = package->build_deps;

    sprintf(handle->log_message, "Build dependencies for %s: \0", package->name);
    japml_log(handle, Information, handle->log_message);

    while (it)
    {
        sprintf(handle->log_message, "  %s", ((japml_package_t*)it->data)->name);
        japml_log(handle, Information, handle->log_message);
    }

    sprintf(handle->log_message, "Installing build dependencies for %s", package->name);
    japml_log(handle, Information, handle->log_message);

    // Actually install the build deps
    japml_install_packages(handle, package->build_deps);

    sprintf(handle->log_message, "Build dependencies for %s installed", package->name);
    japml_log(handle, Information, handle->log_message);
}

void japml_post_install(japml_handle_t* handle, japml_package_t* package)
{
    // * Execute post install script
    japml_list_t* it = package->post_install;

    if (it)
    {
        sptrinf(handle->log_message, "Running post install script for %s", package->name);
        japml_log(handle, Debug, handle->log_message);
    }

    while (it)
    {
        if (system((char*)it->data) == -1)
        {
            sprintf(handle->log_message, "Command failed in post-install script for %s", package->name);
            japml_throw_error(handle, install_error, handle->log_message);
        }
        it = japml_list_next(it);
    }
}