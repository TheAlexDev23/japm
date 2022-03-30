#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "../Errors/errors.h"
#include "HTTP/http.h"

// Some hardcoded values that are used in the program
#define MAX_PACKAGE_FILE_SIZE 10240
#define MAX_INSTALLED_PACKAGES_FILE_SIZE 131072
#define MAX_COMMANDS_NUMBER 1000

int install_single_package(char *package_name);
struct package_information parse_package_information(char *package_name);
void install_package_to_system(struct package_information package_info);
int check_if_package_installed(char *package_name);
void add_package_to_installed_packages(char *package_name);

struct package_information
{
    json_object *name;
    json_object *dependencies;
    json_object *installation;
};

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
    if (check_if_package_installed(package_name))
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
    struct package_information pkg_info = parse_package_information(package_name);

    // We install the package to the system using the pkg_info struct
    install_package_to_system(pkg_info);

    add_package_to_installed_packages(package_name);
    printf("Package \"%s\" installed successfully\n", package_name);
}

struct package_information
parse_package_information(char* package_name) 
{
    //The package json file would be saved under /var/cache/japm/package_name
    //We would use the json-c library to parse information from it
    
    //The json file would be like this:
    //{
    //    "name": "package_name",  // The name of the package
    //    "dependencies": [        // Array of dependencies
    //        "package_name",
    //        "package_name",
    //        "package_name"
    //    ],
    //    "install": [             // Array of commands to be executed to install the package
    //      "installation_command",
    //      "installation_command",
    //      "installation_command"
    //    ]
    //}

    char* file_package_name = malloc(sizeof("/var/cache/japm/") + sizeof(package_name));
    strcpy(file_package_name, "/var/cache/japm/");
    strcat(file_package_name, package_name);

    FILE* package_json_file = fopen(file_package_name, "r");
    if (package_json_file == NULL)
    {
        printf("Package \"%s\" not found\n", package_name);
        exit(package_not_found_error);
    }

    char buffer[MAX_PACKAGE_FILE_SIZE];

    fread(buffer, MAX_PACKAGE_FILE_SIZE, 1, package_json_file);
    fclose(package_json_file);

    json_object *parsed_json;  // The json object extracted from the package file
    json_object *name;         // The name of the package
    json_object *dependencies; // Array of dependencies
    json_object *installation; // String of installation commands

    parsed_json = json_tokener_parse(buffer);

    // Gets the child objects from the main json object
    json_object_object_get_ex(parsed_json, "name", &name);
    json_object_object_get_ex(parsed_json, "dependencies", &dependencies);
    json_object_object_get_ex(parsed_json, "install", &installation);

    // If the package is in a wrong format. (it doesnt have a name or a installation instruction or dependencies object) the we inform the user about it
    if (name == NULL || dependencies == NULL || installation == NULL)
    {
        printf("Package Corrupted, Aborting...\n");
        exit(package_corrupted_error);
    }

    // We create a package_information struct to store the information we got from the json file
    struct package_information package_information;
    package_information.name = name;
    package_information.dependencies = dependencies;
    package_information.installation = installation;

    // We return the package_information struct
    return package_information;
}

void 
install_package_to_system(struct package_information package_info)
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
    }

    //We need to execute the commands array in the package.json file
    //We get the number of commands
    int commands_number = json_object_array_length(package_info.installation);
    //We iterate over the commands array
    for (int i = 0; i < commands_number; i++)
    {
        //We get the current command
        json_object *command = json_object_array_get_idx(package_info.installation, i);
        
        //TODO: Find a way of suppressing the output of the command 
        // We execute the command
        system(json_object_get_string(command));
    }
}

void
add_package_to_installed_packages(char* package_name)
{
    // This would add package_name to the installed_packages file on /var/japm/installed_packages
    // We open the installed_packages file and append the package_name to it
    FILE* installed_packages_file = fopen("/var/japm/installed_packages", "a");
    fprintf(installed_packages_file, "%s\n", package_name);
    fclose(installed_packages_file);
}

int
check_if_package_installed(char* package_name)
{
    start_again: ;
    // This function would check if the package is already installed
    // We check if the package name is in the /var/japm/installed_packages file
    // If it is then we return 1
    // If it is not then we return 0
    FILE* installed_packages_file = fopen("/var/japm/installed_packages", "r");
    if (installed_packages_file == NULL)
    {
        system("mkdir -p /var/japm");
        system("touch /var/japm/installed_packages");
        goto start_again;
    }

    char buffer[MAX_INSTALLED_PACKAGES_FILE_SIZE];
    // We read the installed_packages file to end of file
    fread(buffer, MAX_INSTALLED_PACKAGES_FILE_SIZE, 1, installed_packages_file);
    fclose(installed_packages_file);

    // We check if the package_name is in the installed_packages file
    if (strstr(buffer, package_name) != NULL)
    {
        // The package is already installed
        return 1;
    }
    else
    {
        // The package is not installed
        return 0;
    }
}
