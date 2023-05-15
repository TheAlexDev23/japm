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
#include "update.h"
#include "search.h"

/* Unlike the name might sugest, it doesn't directly create a japml_action_t, rather call inidivdual install functions of JAPM */
int perform_action(japml_handle_t* handle, japml_parse_parameters_t* parameters)
{
    int err = 0;
	if (parameters->package_action == NULL)
	{
		return err;
	}

    bool sw = false;
    if (parameters->update_systemwide != NULL && *(parameters->update_systemwide))
    {
        sw = true;
    }

    bool rr = false;
    if (parameters->remove_recursive != NULL && *(parameters->remove_recursive))
    {
        rr = true;
    }

	switch(*(parameters->package_action))
	{
		case japml_package_install:
            err = install_packages(handle, parameters->packages);
			break;
		case japml_package_remove:
            err = remove_packages(handle, parameters->packages, rr);
			break;
		case japml_package_update:
            err = update_packages(handle, parameters->packages, sw);
			break;
		case japml_package_search:
            err = search_packages(handle, parameters->packages);
			break;
	}

    return err;
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
        japml_pars_params_free(parameters);
		return -1;
	}

	if (perform_action(handle, parameters))
    {
        japml_throw_error(handle, custom_error_error, "Issue performing action, quiting now");
    }

    // ? Maybe add reference to parse parameters in handle and free upon exit
    japml_pars_params_free(parameters);

    japml_log(handle, Information, "Press any key to exit");

    getch();

    japml_exit(handle);

    return 0;
}