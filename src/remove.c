#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

#include "IO/local-repo.h"
#include "IO/term.h"
#include "package.h"
#include "errors.h"

void remove_package_from_system(package pkg, char *package_name);
void check_if_remove_breaks_dependency(char *package_name, bool rec);
void post_install(const char *package_name);

void remove_package_no_dep_check(char *package_name)
{
    // We get the package from the local repository
    // ! It's important to know that even though the package's dependency, removal and update instructions in the json object are usually arrays in this case they are strings
    printf("\033[0;32m==> Preparing to remove the package...\n");
    package pkg = get_package_from_local_repo(package_name);

    // We remove the package from the system
    printf("\033[0;32m==> Removing the package...\n");
    remove_package_from_system(pkg, package_name);

    printf("\033[0;32m==> Refreshing packages...\n");
    post_install(json_object_get_string(pkg.name));

    // We remove the package from the local repository
    printf("\033[0;32m==> Updating the local repository...\n");
    remove_package_from_local_repository(package_name);
    reset();
}

void remove_package_rec(char *package_name)
{
    // We get the package from the local repository
    // ! It's important to know that even though the package's dependency, removal and update instructions in the json object are usually arrays in this case they are strings
    printf("\033[0;32m==> Preparing to remove the package...\n");
    package pkg = get_package_from_local_repo(package_name);

    // We check if removing the package might break any depenencies
    check_if_remove_breaks_dependency(package_name, true);

    // We remove the package from the system
    printf("\033[0;32m==> Removing the package...\n");
    remove_package_from_system(pkg, package_name);

    printf("\033[0;32m==> Refreshing packages...\n");
    post_install(json_object_get_string(pkg.name));

    // We remove the package from the local repository
    printf("\033[0;32m==> Updating the local repository...\n");
    remove_package_from_local_repository(package_name);
    reset();
}

void remove_package(char *package_name)
{
    // We get the package from the local repository
    // ! It's important to know that even though the package's dependency, removal and update instructions in the json object are usually arrays in this case they are strings
    printf("\033[0;32m==> Preparing to remove the package...\n");
    package pkg = get_package_from_local_repo(package_name);

    // We check if removing the package might break any depenencies
    check_if_remove_breaks_dependency(package_name, false);

    // We remove the package from the system
    printf("\033[0;32m==> Removing the package...\n");
    remove_package_from_system(pkg, package_name);

    printf("\033[0;32m==> Refreshing packages...\n");
    post_install(json_object_get_string(pkg.name));

    // We remove the package from the local repository
    printf("\033[0;32m==> Updating the local repository...\n");
    remove_package_from_local_repository(package_name);
    reset();
}

void post_install(const char *package_name)
{
    // We would iterate through all package's used_by file and remove the package_name from the used_by file
    // We iterate for each folder under /var/japm/packages/

    // We open the /var/japm/packages/ folder
    DIR *dir = opendir("/var/japm/packages/");

    if (dir == NULL)
    {
        fprintf(stderr, "\033[0;31m==> Something went wrong...\n");
        reset();
        exit(unkown_error);
    }

    struct dirent *de = readdir(dir);
    
    while (de != NULL && strcmp(de->d_name, ".") != 0)
    {
        printf("\033[0;32m    ==> Updating %s...\n", de->d_name);
        // We open the package's used_by file
        char *used_by_file_dir = malloc(sizeof(char) * (strlen("/var/japm/packages/") + strlen(de->d_name) + strlen("/used_by") + 1));
        strcpy(used_by_file_dir, "/var/japm/packages/");
        strcat(used_by_file_dir, de->d_name);
        strcat(used_by_file_dir, "/used_by");

        FILE *used_by_file = fopen(used_by_file_dir, "r");

        if (used_by_file == NULL)
        {
            fprintf(stderr, "\n\033[31m==> Something went wrong...\n");
            reset();
            exit(unkown_error);
        }

        // We read line by line the used_by file and write it to a temporary file
        // Unless the line we are reading is the package_name we want to remove, then we would just skip it
        char *temp_file_dir = malloc(sizeof(char) * (strlen(used_by_file_dir) + strlen("_temp") + 1));
        strcpy(temp_file_dir, used_by_file_dir);
        strcat(temp_file_dir, "_temp");

        FILE *temp_file = fopen(temp_file_dir, "w");

        if (temp_file == NULL)
        {
            fprintf(stderr, "\n\033[31m==> Something went wrong...\n");
            reset();
            exit(unkown_error);
        }

        char line[5000];

        while (fgets(line, sizeof(line), used_by_file) != NULL)
        {
            line[strlen(line) - 1] = '\0';

            if (strcmp(line, package_name) != 0)
            {
                fprintf(temp_file, "%s\n", line);
            }
        }

        // We replace the used_by file with the temporary file
        fclose(used_by_file);
        fclose(temp_file);

        char *command = malloc(sizeof(char) * (strlen("mv ") + strlen(temp_file_dir) + strlen(" ") + strlen(used_by_file_dir) + 1));
        strcpy(command, "mv ");
        strcat(command, temp_file_dir);
        strcat(command, " ");
        strcat(command, used_by_file_dir);

        system(command);

        de = readdir(dir);
    }
}

void 
check_if_remove_breaks_dependency(char *package_name, bool rec)
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

			if (rec)
			{
				remove_package_rec(line_buffer);
			}
			else
			{
				fprintf(stderr, "\033[31mRemoving the package breaks the dependency of %s\n", line_buffer);

				reset();

				free(used_by);
				fclose(used_by_file);

				exit(dependency_break_error);
			}
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

void remove_package_from_system(package pkg, char *package_name)
{
	printf("\n\n");
	reset();
    
	// We remove the package from the system
    const char *remove_instructions = json_object_get_string(pkg.remove);
    system(remove_instructions);

    char *package_folder = malloc(sizeof(char) * (strlen("rm -rf ") + strlen("/var/japm/packages/") + strlen(package_name) + 1));

    strcpy(package_folder, "rm -rf /var/japm/packages/");
    strcat(package_folder, package_name);

    system(package_folder);
	
	printf("\n\n");
	reset();
}

