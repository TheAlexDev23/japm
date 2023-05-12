#ifndef _JAPML_REMOVE_H_INCLUDED
#define _JAPML_REMOVE_H_INCLUDED

#include "japml.h"
#include "handle.h"
#include "list.h"
#include "package.h"

int japml_remove_packages(japml_handle_t* handle, japml_list_t* packages);
int japml_remove_single_package(japml_handle_t* handle, japml_package_t* package);

#endif