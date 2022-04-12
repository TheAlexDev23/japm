#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "errors.h"
#include "IO/term.h"
#include "IO/local-repo.h"
#include "package.h"
#include "internet.h"

// Some hardcoded values that are used in the program
#define MAX_PACKAGE_FILE_SIZE 10240
#define MAX_INSTALLED_PACKAGES_FILE_SIZE 131072
#define MAX_COMMANDS_NUMBER 1000

void install_package_to_system(package package_info, char *package_name);
void install_files(json_object *files, char *package_name);

int install_single_package(char *package_name)
{
    if (check_if_package_is_installed(package_name))
    {
		reset();
        fprintf(stderr, "==> Package '%s' is already installed\n\n", package_name);
		reset();
        return 0;
    }

    // The package metadata would be located in https://raw.githubusercontent.com/TheAlexDev23/japm-official-packages/main/packages/package_name/package.json
    // We need to create a url string according the the package name and then call the http_req function to get the response code
    // If the response code is 200 then we need to download the package file and then install it to the system
    // If the response code is 404 then we need to print an error message and exit with the package_not_found_error code
    // If the response code is anything else then we need to print an error message and exit with the unkown_error code
    char *url = malloc(sizeof(char) * (strlen("https://raw.githubusercontent.com/TheAlexDev23/japm-official-packages/main/packages/") + strlen(package_name) + strlen("/package.json") + 1));
    strcpy(url, "https://raw.githubusercontent.com/TheAlexDev23/japm-official-packages/main/packages/");
    strcat(url, package_name);
    strcat(url, "/package.json");

    printf("\033[0;32m==> Downloading the package metadata...\n");

    int http_res = http_req(url);

    if (http_res == 404)
    {
        // Package is not found on the repo
        fprintf(stderr, "\033[0;31m==> Package \"%s\" not found \n", package_name);
        reset();
        exit(package_not_found_error);
    }
    else if (http_res != 200)
    {
        // Some unkown or server error happened

        fprintf(stderr, "\033[31m==> Something Went Wrong...\033[0m\n");
        reset();
        exit(unkown_error);
    }

    // Download the package file from the repo
    download_package(url, package_name);

    // Parse the package.json file downloaded from the repo
    char *json_file_location = malloc(sizeof(char) * (strlen("/var/cache/japm/") + strlen(package_name) + 1));
    strcpy(json_file_location, "/var/cache/japm/");
    strcat(json_file_location, package_name);
    FILE *json_file = fopen(json_file_location, "r");

    printf("\033[0;32m==> Parsing the package metadata...\n");

    if (json_file == NULL)
    {
        fprintf(stderr, "\033[0;31m==> Couldn't open the package.json file\n");
        reset();
        exit(unkown_error);
    }

    package pkg_info = parse_package_information(json_file);
	
	// Install the needed files (mentioned in the json file)
	install_files(pkg_info.files, package_name);

    // We install the package to the system using the pkg_info struct
    printf("\033[0;32m==> Installing the package...\n");
    install_package_to_system(pkg_info, package_name);

    printf("\033[0;32m==> Updating local repo...\n");
    add_package_to_local_repo(pkg_info, package_name);

    add_package_to_installed_packages(package_name);
    printf("\033[0;32m==> Package \"%s\" installed successfully\n", package_name);
}

void install_files(json_object *files, char *package_name)
{
	char *dir_to_create = malloc(sizeof(char) * (strlen("/tmp/japm/") + strlen(package_name) + 1));
	strcpy(dir_to_create, "/tmp/japm/");
	strcat(dir_to_create, package_name);

	char *command_dir_to_create = malloc(sizeof(char) * (strlen("mkdir -p ") + strlen(dir_to_create) + 1));
	char *command_dir_to_remove = malloc(sizeof(char) * (strlen("rm -rf ") + strlen(dir_to_create) + 1));

	strcpy(command_dir_to_create, "mkdir -p ");
	strcat(command_dir_to_create, dir_to_create);
	
	strcpy(command_dir_to_remove, "rm -rf ");
	strcat(command_dir_to_remove, dir_to_create);
	
	printf("\n");

	system(command_dir_to_remove);
	system(command_dir_to_create);
	
	printf("\n");

	free(command_dir_to_create);
	free(command_dir_to_remove);
	
	// They would be installed here: /tmp/japm/<package_name>/<file_name>
	
	// Iterate over all the files that need to be downloaded

	for (int i = 0; i < json_object_array_length(files); i++)
	{
		json_object *file_to_dl = json_object_array_get_idx(files, i);

		json_object *url_json_object = json_object_object_get(file_to_dl, "url");
		json_object *filename_json_object = json_object_object_get(file_to_dl, "file name");

		const char *url = json_object_get_string(url_json_object);
		const char *filename = json_object_get_string(filename_json_object);

		char *directory = malloc(sizeof(char) * (strlen("/tmp/japm/") + strlen(package_name) + strlen("/") + strlen(filename) + 1));

		strcpy(directory, "/tmp/japm/");
		strcat(directory, package_name);
		strcat(directory, "/");
		strcat(directory, filename);

		// Execute the following command:
		// curl <url> -o directory
	
		char *command = calloc(strlen("curl ") + strlen(url) + strlen(" -o ") + strlen(directory) + 1, sizeof(char));

		strcpy(command, "curl ");
		strcat(command, url);
		strcat(command, " -o ");
		strcat(command, directory);

		printf("\033[32m==> Getting file: %s (%s)\n\n", filename, url);
		reset();

		system(command);

		printf("\n\n");
		
		free(directory);
	}
}

char* string_replace(char* source, size_t sourceSize, char* substring, char* with) {
    char* substring_source = strstr(source, substring);
    if (substring_source == NULL) {
        return NULL;
    }

    if (sourceSize < strlen(source) + (strlen(with) - strlen(substring)) + 1) {
		fprintf(stderr, "\033[31mPackage corrupted, aborting...\n");
		reset();
		exit(package_corrupted_error);
    }

    memmove(
        substring_source + strlen(with),
        substring_source + strlen(substring),
        strlen(substring_source) - strlen(substring) + 1
    );

    memcpy(substring_source, with, strlen(with));
    return substring_source + strlen(with);
}

void install_package_to_system(package package_info, char *package_name)
{
	reset();
	printf("\n");
    // This function would install the package and it's dependencies (if dep true) to the system
    // We install all the dependencies recursevly using the install_single_package function
	
	// We get the number of dependencies
	int dependencies_number = json_object_array_length(package_info.dependencies);

	// We print out the dependencies to the user
	if (dependencies_number != 0)
		printf("\033[0;32m==> Dependencies of %s:\n", json_object_get_string(package_info.name));
	
	reset();

	// We iterate over the dependencies array
	for (int i = 0; i < dependencies_number; i++)
	{
		// We get the current dependency
		json_object *dependency = json_object_array_get_idx(package_info.dependencies, i);

		// We print the dependency to the user
		printf("    - %s\n", json_object_get_string(dependency));
	}

	// We iterate over all the dependencies and install them
	for (int i = 0; i < dependencies_number; i++)
	{
		// We get the name of the dependency
		json_object *dependency_name = json_object_array_get_idx(package_info.dependencies, i);
		// We install the dependency
		install_single_package(json_object_get_string(dependency_name));

		// We would also need to append in the dependencies directory used_by file the name of the package being installed
		const char *dependency_name_str = json_object_get_string(dependency_name);
		char *dependency_folder_dir = malloc(sizeof(char) * (strlen("/var/japm/packages/") + strlen(dependency_name_str) + 1));
		strcpy(dependency_folder_dir, "/var/japm/packages/");
		strcat(dependency_folder_dir, dependency_name_str);

		// We open the used_by file of the dependency
		FILE *used_by_file = fopen(strcat(dependency_folder_dir, "/used_by"), "a");

		// We append the name of the package being installed to the used_by file
		fprintf(used_by_file, "%s\n", package_name);
	}

    // We need to execute the commands array in the package.json file
    // We get the number of commands
    int commands_number = json_object_array_length(package_info.install);
    // We iterate over the commands array
    for (int i = 0; i < commands_number; i++)
    {
        // We get the current command
        json_object *command = json_object_array_get_idx(package_info.install, i);

		char *str_command = json_object_get_string(command);

		char *working_dir = malloc(sizeof(char) * (strlen("/tmp/japm/") + strlen(package_name) + 1));
		strcpy(working_dir, "/tmp/japm/");
		strcat(working_dir, package_name);
		
		string_replace(str_command, strlen(str_command) + strlen(working_dir), "${package_dir}", working_dir);
        // We execute the command
        system(str_command);
    }
	
	printf("\n");
}

