#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IO/local-repo.h"
#include "package.h"
#include "errors.h"

void remove_package_from_system(package pkg);
void check_if_remove_breaks_dependency(char *package_name);

void remove_package(char *package_name)
{
    // We get the package from the local repository
    printf("==> Preparing to remove the package...\n");
    package pkg = get_package_from_local_repo(package_name);

    // We check if removing the package might break any depenencies
    check_if_remove_breaks_dependency(package_name);

    // We remove the package from the system
    printf("==> Removing the package...\n");
    remove_package_from_system(pkg);

    // We remove the package from the local repository
    printf("==> Updating the local repository...\n");
    remove_package_from_local_repository(json_object_get_string(pkg.name));
}

void check_if_remove_breaks_dependency(char *package_name)
{
    // We get the used_by file from the package we want to remove (/var/japm/packages/<package_name>/used_by)
    char *used_by = malloc(sizeof(char) * (strlen("/var/japm/packages/") + strlen(package_name) + strlen("/used_by") + 1));

    strcpy(used_by, "/var/japm/packages/");
    strcat(used_by, package_name);
    strcat(used_by, "/used_by");

    // We open the used_by file
    FILE *used_by_file = fopen(used_by, "r");

    if (used_by_file == NULL)
    {
        return;
    }

    char buffer;
    char line_buffer[MAX_PACKAGE_NAME_SIZE];

    int count = 0;

    while (fread(&buffer, 1, 1, used_by_file))
    {
        if (buffer == '\n')
        {
            line_buffer[count] = '\0';

            printf("\033[31mRemoving the package breaks the dependency of %s\n", line_buffer);

            free(used_by);
            fclose(used_by_file);

            exit(dependency_break_error);
        }
        else
        {
            line_buffer[count] = buffer;
            count++;
        }
    }

    free(used_by);
    fclose(used_by_file);
}

void remove_package_from_system(package pkg)
{
    // We remove the package from the system
    char *remove_instructions = json_object_get_string(pkg.remove);
    printf("%s\n", remove_instructions);
    system(remove_instructions);

    char *package_folder = malloc(sizeof(char) * (strlen("/var/japm/packages/") + strlen(json_object_get_string(pkg.name)) + 1));

    strcpy(package_folder, "/var/japm/packages/");

    system(strcat("rm -rf ", package_folder));
}
