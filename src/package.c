#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "package.h"

void add_package_to_installed_packages(char *package_name)
{
    // This would be done by creating a folder with the package name under /var/japm/packages/
    char *directory = malloc(sizeof(char) * (strlen("/var/japm/packages/") + strlen(package_name) + 1));
    strcpy(directory, "/var/japm/packages/");
    strcat(directory, package_name);

    char *command = malloc(strlen("mkdir -p ") + strlen(directory) + 1);
    strcpy(command, "mkdir -p ");
    strcat(command, directory);

    system(command);
}

// Returns 0 if the package is not installed
// Returns 1 if the package is installed
int check_if_package_is_installed(char *package_name)
{
    char *directory = malloc(strlen("/var/japm/packages/") + strlen(package_name) + 1);
    strcpy(directory, "/var/japm/packages/");
    strcat(directory, package_name);

    FILE *f = fopen(directory, "r");

    if (f == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
