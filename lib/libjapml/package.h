#ifndef _JAPML_PACKAGE_H_INCLUDED
#define _JAPML_PACKAGE_H_INCLUDED

#include "japml.h"
#include "list.h"
#include "handle.h"

#define MAX_PACKAGE_NAME_LENGTH 128
#define MAX_PACKAGE_DESCRIPTION_LENGTH 1042

#define JAPML_PACKAGE_DIR_VAR "${package_dir}"

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

japml_package_t* japml_package_create_empty();

// Fully frees a pacakge and it's children
void japml_package_free(japml_package_t* package);

// Frees all the packages in packages and the list itself
void japml_package_free_list(japml_list_t* packages);

#define PACKAGE_USED_BY_TMP "/tmp/japml/depending_pkg_tmp"

/* 
* Essentially will mark all the packages that package depends on in each's used_by file
*/
void japml_package_mark_dependencies(japml_handle_t* handle, japml_package_t* package);

/* Sets depender as a package that depends on pkg */
void japml_package_append_depender(japml_handle_t* handle, char* pkg, char* depender);

/* 
* Generally, if a package is deleted, we should also remove the "dependency link" between the package and the packages it depends on
* This will unlink depender from each of depender's dependency
*/
void japml_package_remove_depender(japml_package_t* depender);

/* 
* Sets depending_packages of package to the packages that depend on it.
* This function should be called every time dpeneding_packages are updated and/or need to be accessed.
*/
void japml_package_get_depending(japml_handle_t* handle, japml_package_t* package);

/* 
* Will ensure that package does not exist in the list before adding it
* Returns 1 if package is already in list, 0 otherwise
 */
int japml_package_add_to_list_no_rep(japml_list_t** list, japml_package_t* package);

/* Will print a list of packages */
void japml_package_print_list(japml_handle_t* handle, japml_list_t* packages);

/* Will print information about package */
void japml_package_print_details(japml_handle_t* handle, japml_package_t* package);

#endif