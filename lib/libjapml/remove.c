#include "remove.h"
#include "japml.h"
#include "list.h"
#include "system.h"
#include "package.h"
#include "handle.h"
#include "log.h"
#include "db.h"
#include "japmlcurses.h"
#include "file.h"

int japml_remove_packages(japml_handle_t* handle, japml_list_t* packages)
{
    japml_list_t* it = packages;

    japml_ncurses_pb_set_lim(handle, handle->ncurses_pb_lim + japml_list_length(packages));

    while (it)
    {
        japml_package_t* pkg = (japml_package_t*)it->data;

        if (japml_remove_single_package(handle, pkg))
        {
            japml_throw_error(handle, install_error, "Error removing package");
            return 1;
        }

        japml_ncurses_pl_add(handle, pkg, japml_package_remove);
        japml_ncurses_pb_add(handle, 1);

        it = japml_list_next(it);
    }
}

int japml_remove_single_package(japml_handle_t* handle, japml_package_t* package)
{
    sprintf(handle->log_message, "Removing package %s", package->name);
    japml_log(handle, Information, handle->log_message);

    char* pkg_dir = japml_get_package_directory(package);
    if (japml_run_instructions(package->remove, pkg_dir))
    {
        japml_throw_error(handle, install_error, "Remove instructions cannot be executed successfully");
        return -1;
    }

    sprintf(handle->log_message, "Package %s removed succesfully", package->name);
    japml_log(handle, Information, handle->log_message);

    japml_log(handle, Information, "Updating local db...");
    japml_db_local_remove_package(handle, package);
    japml_package_remove_depender(handle, package);

    japml_delete_dir_rf(pkg_dir);
    free(pkg_dir);

    japml_log(handle, Information, "Done");
}