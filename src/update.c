#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "errors.h"
#include "remove.h"
#include "install.h"
#include "IO/term.h"

void update_package(char *package_name)
{
	/*
	 * This function will first remove the package from the system and install it's latest version from the repo
	 * */

	//Remove the package
	
	
	printf("\033[0;32m==> Reinstalling the package...\n\n");
	remove_package(package_name);

	printf("\n\n");

	install_single_package(package_name);

	printf("\033[0;32m==> Package %s updated\n", package_name);

	reset();
}

void update_system()
{
	// For each package in /var/japm/packages/<package_name> update it using update_package

	DIR *dir = opendir("/var/japm/packages/");

    if (dir == NULL)
    {
        fprintf(stderr, "\033[31m==> Something went wrong...\n");
        reset();
        exit(unkown_error);
    }

    struct dirent *de = readdir(dir);
    
    while (de != NULL && strcmp(de->d_name, ".") != 0)
    {
		update_package(de->d_name);
		de = readdir(dir);
	}
}

