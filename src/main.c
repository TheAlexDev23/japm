#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <libjapml/japml.h>
#include <libjapml/parser.h>
#include <libjapml/handle.h>
#include <libjapml/init.h>
#include <libjapml/package.h>
#include <libjapml/exit.h>
#include <libjapml/log.h>

#define TESTING false

#include "devel.h"
#include "install.h"
#include "remove.h"

/* Unlike the name might sugest, it doesn't directly create a japml_action_t, rather call inidivdual install functions of JAPM */
void perform_action(japml_handle_t* handle, japml_parse_parameters_t* parameters)
{
	if (parameters->package_action == NULL)
	{
		return;
	}

	switch(*(parameters->package_action))
	{
		case japml_package_install:
            install_packages(handle, parameters->packages);
			break;
		case japml_package_remove:
            remove_packages(handle, parameters->packages, parameters->remove_recursive);
			break;
		case japml_package_update:
			break;
		case japml_package_search:
			break;
	}
}

int main(int argc, char **argv)
{
	if (TESTING)
	{
		test_japml();
	}

	japml_parse_parameters_t* parameters = japml_parse_input(argc, argv);

	if (parameters == NULL)
	{
		return -1;
	}

	japml_handle_t* handle = japml_init(parameters);

	if (handle == NULL)
	{
		return -1;
	}

	perform_action(handle, parameters);

    getch();

    japml_exit(handle);

    return 0;
}