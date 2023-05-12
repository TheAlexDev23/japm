#ifndef _JAPML_PACKAGE_H_INCLUDED
#define _JAPML_PACKAGE_H_INCLUDED

#include "japml.h"
#include "list.h"
#include "handle.h"

#define MAX_PACKAGE_NAME_LENGTH 128
#define MAX_PACKAGE_DESCRIPTION_LENGTH 1042

enum _japml_package_action
{
    japml_package_search,
    japml_package_install,
    japml_package_update,
    japml_package_remove
};

struct _japml_package
{
	char* name;
	char* description;
	char* version;

    // * Dependencies
    /* Dependencies to be able to build this package */
	japml_list_t* build_deps;
    /* Dependencies for this package to work */
	japml_list_t* deps;
    /* Packages that depend on it */
    japml_list_t* depending_packages;

    // * Installation
    /* List of instructions executed before installation */
    japml_list_t* pre_install;
    /* List of install instructions for the package */
    japml_list_t* install;
    /* List of files required for install or usage */
    japml_list_t* files;
    /* List of instructions executed after installation */
    japml_list_t* post_install;

    // * Removal
    /* List of removal instructions for the package */
    japml_list_t* remove;
};

// Fully frees a pacakge and it's children
void japml_free_package(japml_package_t* package);

#define PACKAGE_USED_BY_TMP "/tmp/japml/depending_pkg_tmp"

/* Sets depender as a package that depends on pkg */
void japml_append_depenending_package(japml_handle_t* handle, japml_package_t* pkg, japml_package_t* depender);

/* 
* Generally, if a package is deleted, we should also remove the "dependency link" between the package and the packages it depends on
* THis will cycle thorugh all deps of depender and remove depender->name from their used_by
*/
void japml_remove_depending_package(japml_handle_t* handle, japml_package_t* depender);

/* 
* Sets depending_packages of package to the packages that depend on it.
* This function should be called every time dpeneding_packages are updated and need to be accessed.
*/
void japml_get_depending_packages(japml_handle_t* handle, japml_package_t* package);

/* 
* Will ensure that package does not exist in the list before adding it
* Returns 1 if package is already in list, 0 otherwise
 */
int japml_add_package_to_list_no_repeat(japml_handle_t* handle, japml_list_t** list, japml_package_t* package);

/* Will print a list of packages */
void japml_print_package_list(japml_handle_t* handle, japml_list_t* packages);

#endif