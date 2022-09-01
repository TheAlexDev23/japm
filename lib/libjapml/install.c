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

void japml_pre_install(japml_handle_t* handle, japml_package_t* package)
{
    // * Run pre install script
    japml_list_t* it = package->pre_install;

    if (it)
    {
        sprintf(handle->log_message, "Running pre-install script for %s", package->name);
        japml_log(handle, Information, handle->log_message);
    }

    while (it)
    {
        system((char*)it->data);
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

    // Install build deps
    japml_install_packages(handle, package->build_deps);

    sprintf(handle->log_message, "Build dependencies for %s installed", package->name);
    japml_log(handle, Information, handle->log_message);
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
        if (system(cmd) != -1)
        {
            japml_throw_error(handle, install_error, NULL);
        }
    }
}