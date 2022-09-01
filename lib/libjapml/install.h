#ifndef _INSTALL_H_INCLUDED
#define _INSTALL_H_INCLUDED

#include "japml.h"
#include "list.h"

void japml_install_packages(japml_handle_t* handle, japml_list_t* packages);
void japml_install_single_package(japml_handle_t* handle, japml_package_t* package);

void japml_pre_install(japml_handle_t* handle, japml_package_t* package);

#endif