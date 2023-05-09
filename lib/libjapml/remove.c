#include "remove.h"
#include "japml.h"
#include "list.h"
#include "system.h"
#include "package.h"
#include "handle.h"
#include "log.h"
#include "db.h"

int japml_remove_packages(japml_handle_t* handle, japml_list_t* packages)
{
    japml_list_t* it = packages;

    while (it)
    {
        japml_package_t* pkg = (japml_package_t*)it->data;

        if (japml_remove_single_package(handle, pkg))
        {
            japml_throw_error(handle, install_error, "Error removing package");
            return 1;
        }

        it = japml_list_next(it);
    }
}

int japml_remove_single_package(japml_handle_t* handle, japml_package_t* package)
{
    sprintf(handle->log_message, "Removing package %s", package->name);
    japml_log(handle, Information, handle->log_message);

    japml_run_instructions(package->remove);

    sprintf(handle->log_message, "Package %s removed succesfully", package->name);
    japml_log(handle, Information, handle->log_message);

    japml_log(handle, Information, "Updating local db...");
    japml_remove_package_from_local_db(handle, package);
    japml_remove_depending_package(handle, package);
    japml_log(handle, Information, "Done");
}