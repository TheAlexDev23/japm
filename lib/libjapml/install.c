#include "japml.h"
#include "list.h"
#include "install.h"

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

        it = japml_list_next(it);
    }
}

void japml_install_single_package(japml_handle_t* handle, japml_package_t* package)
{
    
}