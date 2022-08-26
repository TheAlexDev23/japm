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

    //* Installation
    /* List of install instructions for the package */
    japml_list_t* install;
    /* List of files required for install or usage */
    japml_list_t* files;

    //* Removal
    /* List of removal instructions for the package */
    japml_list_t* remove;
};

#endif