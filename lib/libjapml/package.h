#ifndef _PACKAGE_H_INCLUDED
#define _PACKAGE_H_INCLUDED

#include "japml.h"

#define MAX_PACKAGE_NAME_LENGTH 128
#define MAX_PACKAGE_DESCRIPTION_LENGTH 1042

struct _japml_package
{
	char name[MAX_PACKAGE_NAME_LENGTH];
	char description[MAX_PACKAGE_DESCRIPTION_LENGTH];
	char version[25];

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

// ! IMPLEMENT
void japml_free_package(japml_package_t* package);

/* Sets depender as a package that depends on pkg */
void japml_append_depenending_packages(japml_handle_t* handle, japml_package_t* pkg, japml_package_t* depender);
/* 
* Sets depending_packages of package to the packages that depend on it.
* This function should be called every time dpeneding_packages are updated and need to be accessed.
*/
void japml_get_depending_packages(japml_handle_t* handle, japml_package_t* package);

/* 
* Will insure that package does not exist in the list before adding it
* Returns 1 if package is already in list, 0 otherwise
 */
int japml_add_package_to_list_no_repeat(japml_handle_t* handle, japml_list_t** list, japml_package_t* package);

#endif