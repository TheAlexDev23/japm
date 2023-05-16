#include <libjapml/db.h>

#include "install.h"
#include "remove.h"

#include "update.h"

int update_packages(japml_handle_t* handle, japml_list_t* targets, bool systemwide)
{
    japml_log(handle, Information, "Packages will be deleted recursively and reinstalled");

    if (systemwide)
    {
        // We ignore the passed targets since we are doing a system wide update
        targets = japml_db_local_get_all_packages_name(handle);
    }

    if (targets == NULL)
    {
        japml_log(handle, Information, "There's nothing to do");
        return 0;
    }

    if (remove_packages(handle, targets, true))
    {
        japml_list_free_data(targets);
        return -1;
    }

    if (install_packages(handle, targets))
    {
        japml_list_free_data(targets);
        return -1;
    }

    japml_list_free_data(targets);

    return 0;
}