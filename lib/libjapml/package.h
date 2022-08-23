#ifndef _PACKAGE_H_INCLUDED
#define _PACKAGE_H_INCLUDED

#include "japml.h"

struct _japml_package
{
	char *name;
	char *description;
	char *version;

	japml_list_t depends; // list of japml_package_t that this package depends on
};

#endif