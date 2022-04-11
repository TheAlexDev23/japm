#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "errors.h"
#include "remove.h"
#include "install.h"
#include "IO/term.h"

void pre_update(char *package_name)
{
	// Copy the package's used_by file to a tmp location
	char *used_by_file_loc = malloc(sizeof(char) * (strlen("/var/japm/packages/") + strlen(package_name) + strlen("/used_by") + 1));

	strcpy(used_by_file_loc, "/var/japm/packages/");
	strcat(used_by_file_loc, package_name);
	strcat(used_by_file_loc, "/used_by");

	char *command = malloc(sizeof(char) * (strlen("cat   > /tmp/japm_package_update_temp_file") + strlen(used_by_file_loc) + 1));

	strcpy(command, "cat ");
	strcat(command, used_by_file_loc);
	strcat(command, " > /tmp/japm_package_update_temp_file");
	
	system(command);

	free(command);
}

void post_update(char *package_name)
{
	
	// Copy the package's used_by file to a tmp location
	char *used_by_file_loc = malloc(sizeof(char) * (strlen("/var/japm/packages/") + strlen(package_name) + strlen("/used_by") + 1));

	strcpy(used_by_file_loc, "/var/japm/packages/");
	strcat(used_by_file_loc, package_name);
	strcat(used_by_file_loc, "/used_by");
	
	char *command = malloc(sizeof(char) * (strlen("cat /tmp/japm_package_update_temp_file > ") + strlen(used_by_file_loc) + 1));

	strcpy(command, "cat /tmp/japm_package_update_temp_file > ");
	strcat(command, used_by_file_loc);

	system(command);

	free(command);
}


void update_package(char *package_name)
{
	/*
	 * This function will first remove the package from the system and install it's latest version from the repo
	 * */


	printf("\033[0;32m==> Updating package %s\n\n", package_name);

	printf("\033[0;32m==> Preparing for udpate...\n\n");
	pre_update(package_name);

	printf("\033[0;32m==> Reinstalling the package...\n\n");
	remove_package_no_dep_check(package_name);

	printf("\n\n");

	install_single_package(package_name);

	printf("\033[0;32m==> Executing post update\n", package_name);
	post_update(package_name);
	
	printf("\033[0;32m==> Package %s updated succesfully\n", package_name);

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

