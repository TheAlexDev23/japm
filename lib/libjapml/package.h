#ifndef _PACKAGE_H_INCLUDED
#define _PACKAGE_H_INCLUDED

#include "japml.h"

struct _japml_package
{
	char *name;
	char *description;
	char *version;

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

/* Sets the depending_packages to packages that depend on pkg */
void japml_set_dpenending_packages(japml_handle_t* handle, japml_package_t* pkg);

#endif