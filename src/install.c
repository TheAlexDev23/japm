#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "errors.h"
#include "IO/json.h"
#include "package.h"
#include "internet.h"

// Some hardcoded values that are used in the program
#define MAX_PACKAGE_FILE_SIZE 10240
#define MAX_INSTALLED_PACKAGES_FILE_SIZE 131072
#define MAX_COMMANDS_NUMBER 1000

int install_single_package(char *package_name);
void install_package_to_system(package package_info);
void update_local_repository(package pkg);

int 
install_package(int argc, char **argv)
{
    if (check_install_usage(argc, argv))
    {
        exit(wrong_usage_error);
    }

    return install_single_package(argv[2]);
}

int 
install_single_package(char *package_name)
{
    if (check_if_package_is_installed(package_name))
    {
        printf("Package \"%s\" is already installed\n", package_name);
        return 0;
    }

    //The package metadata would be located in https://raw.githubusercontent.com/TheAlexDev23/japm-official-packages/main/packages/package_name/package.json
    //We need to create a url string according the the package name and then call the http_req function to get the response code
    //If the response code is 200 then we need to download the package file and then install it to the system
    //If the response code is 404 then we need to print an error message and exit with the package_not_found_error code
    //If the response code is anything else then we need to print an error message and exit with the unkown_error code
    char* url = malloc(sizeof(char) * (strlen("https://raw.githubusercontent.com/TheAlexDev23/japm-official-packages/main/packages/") + strlen(package_name) + strlen("/package.json") + 1));
    strcpy(url, "https://raw.githubusercontent.com/TheAlexDev23/japm-official-packages/main/packages/");
    strcat(url, package_name);
    strcat(url, "/package.json");

    printf("Downloading the package metadata...\n");

    int http_res = http_req(url);

    if (http_res == 404)
    {
        // Package is not found on the repo
        printf("Package \"%s\" not found \n", package_name);
        exit(package_not_found_error);
    }
    else if (http_res != 200)
    {
        // Some unkown or server error happened
        printf("Something Went Wrong\n...");
        exit(unkown_error);
    }

    //Download the package file from the repo
    download_package(url, package_name);

    // Parse the package.json file downloaded from the repo
    char* json_file_location = malloc(sizeof(char) * (strlen("/var/cache/japm/") + strlen(package_name) + 1));
    strcpy(json_file_location, "/var/cache/japm/");
    strcat(json_file_location, package_name);
    FILE *json_file = fopen(json_file_location, "r");

    printf("Parsing the package metadata...\n");

    if (json_file == NULL)
    {
        printf("Couldn't open the package.json file\n");
        exit(unkown_error);
    }

    package pkg_info = parse_package_information(json_file);

    printf("Installing the package...\n");
    // We install the package to the system using the pkg_info struct
    install_package_to_system(pkg_info);

    // We update the local repository with the package info
    printf("Updating the local repository...\n");
    update_local_repository(pkg_info);

    add_package_to_installed_packages(package_name);
    printf("Package \"%s\" installed successfully\n", package_name);
}

void 
install_package_to_system(package package_info)
{
    // This function would install the package and it's dependencies to the system
    // We install all the dependencies recursevly using the install_single_package function

    // We get the number of dependencies
    int dependencies_number = json_object_array_length(package_info.dependencies);

    // We print out the dependencies to the user
    if (dependencies_number != 0) printf("Dependencies of %s:\n", json_object_get_string(package_info.name));

    // We iterate over the dependencies array
    for (int i = 0; i < dependencies_number; i++)
    {
        // We get the current dependency
        json_object *dependency = json_object_array_get_idx(package_info.dependencies, i);

        // We print the dependency to the user
        printf("    %s\n", json_object_get_string(dependency));
    }

    // We iterate over all the dependencies and install them
    for (int i = 0; i < dependencies_number; i++)
    {
        // We get the name of the dependency
        json_object *dependency_name = json_object_array_get_idx(package_info.dependencies, i);
        // We install the dependency
        install_single_package(json_object_get_string(dependency_name));

        // We would also need to append in the dependencies directory used_by file the name of the package being installed
        char *dependency_name_str = json_object_get_string(dependency_name);
        char *dependency_folder_dir = malloc(sizeof(char) * (strlen("/var/japm/packages/") + strlen(dependency_name_str) + 1));
        strcpy(dependency_folder_dir, "/var/japm/packages/");
        strcat(dependency_folder_dir, dependency_name_str);

        // We open the used_by file of the dependency
        FILE *used_by_file = fopen(strcat(dependency_folder_dir, "/used_by"), "a");
        
        // We append the name of the package being installed to the used_by file
        fprintf(used_by_file, "%s\n", json_object_get_string(package_info.name));
    }

    //We need to execute the commands array in the package.json file
    //We get the number of commands
    int commands_number = json_object_array_length(package_info.install);
    //We iterate over the commands array
    for (int i = 0; i < commands_number; i++)
    {
        //We get the current command
        json_object *command = json_object_array_get_idx(package_info.install, i);
        
        //TODO: Find a way of suppressing the output of the command 
        // We execute the command
        system(json_object_get_string(command));
    }
}

void 
update_local_repository(package pkg)
{
    int add_coma = 1;
    start_again: ;
    //This function would update the local repository of installed packages
    //This is done by appending the name,version,description,update and removal instructions of the package to the /var/japm/repos/local.json file in a json format
    //We need to open the local.json file
    FILE *local_repo_file = fopen("/var/japm/repos/local.json", "a");

    if (local_repo_file == NULL)
    {
        //We create the local.json file
        system("mkdir -p /var/japm/repos");
        system("touch /var/japm/repos/local.json");
        add_coma = 0;
        goto start_again;
    }

    //We need to get the package name, version, description, update and removal instructions from the package.json file
    //We get the name of the package
    char *package_name = json_object_get_string(pkg.name);
    //We get the version of the package
    char *package_version = json_object_get_string(pkg.version);
    //We get the description of the package
    char *package_description = json_object_get_string(pkg.description);
    //We get the update instructions of the package
    char *package_update = json_object_get_string(pkg.update);
    //We get the removal instructions of the package
    char *package_removal = json_object_get_string(pkg.remove);

    //We append the package name, version, description, update and removal instructions to the local.json file
    if (!add_coma)
    {
        // We truncate the last 2 ]} characters of the file
        fseek(local_repo_file, -2, SEEK_END);
        fprintf(local_repo_file, ",\n");

        // We append the package name, version, description, update and removal instructions to the file
        fprintf(local_repo_file, 
                "{\"name\":\"%s\",\"version\":\"%s\",\"description\":\"%s\",\"update\":\"%s\",\"remove\":\"%s\"}", 
                package_name, package_version, package_description, package_update, package_removal);
        
        //We add the last ]} characters to the file
        fprintf(local_repo_file, "]}");
    }
    else
    {
        fprintf(local_repo_file, "{\"packages\":[\n");

        // We append the package name, version, description, update and removal instructions to the file
        fprintf(local_repo_file, 
                "{\"name\":\"%s\",\"version\":\"%s\",\"description\":\"%s\",\"update\":\"%s\",\"remove\":\"%s\"}", 
                package_name, package_version, package_description, package_update, package_removal);
        
        //We add the last ]} characters to the file
        fprintf(local_repo_file, "]}");
    }
}
