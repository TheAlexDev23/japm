#include "install.h"
#include "remove.h"

#include "update.h"

int update_packages(japml_handle_t* handle, japml_list_t* targets)
{
    japml_log(handle, Information, "Packages will be deleted recursively and reinstalled");

    if (remove_packages(handle, targets, true))
    {
        return -1;
    }

    if (install_packages(handle, targets))
    {
        return -1;
    }

    return 0;
}