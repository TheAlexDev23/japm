#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "../Errors/errors.h"
#include "HTTP/http.h"

#define MAX_PACKAGE_FILE_SIZE 10240

int install_single_package(char*package_name);

int
install_package(int argc, char**argv) 
{
    if (check_install_usage(argc, argv))
    {
        return 4;
    }

    return install_single_package(argv[2]);
}

int
install_single_package(char*package_name) 
{
    /* 
    * Downloads package from internet 
    */

    //The package metadata and install instructions would be downloaded only from one repo by now, the official one, on my profile.
    //I might add downloading packages from multiple repos in the future
    char *url = malloc(sizeof("https://raw.githubusercontent.com/TheAlexDev23/japm-official-packages/packages//package.json" + sizeof(package_name))); 
    sprintf(url, "https://raw.githubusercontent.com/TheAlexDev23/japm-official-packages/packages/%s/package.json", package_name);

    //We check the response code. To check if it gives us a not found error or other server errors
    //By this we can know if the package is not found on any repo
    int http_res = http_req(url);

    if (http_res == 404)
    {
        //Package is not found on the repo
        printf("Package \"%s\" not found \n", package_name);
        return 5;
    } else if (http_res != 200) {
        //Some unkown or server error happened
        printf("Something Went Wrong\n...");
        return 10;
    }

    download_package(url, package_name);

    free(url);

    /*
    * Parses package information 
    */

    char*f_package_name = malloc(sizeof("/var/cache/japm/") + sizeof(package_name));
    sprintf(f_package_name, "/var/cache/japm/%s", package_name);
    FILE *package_json_file = fopen(f_package_name, "r");
    free(f_package_name);

    if (package_json_file == NULL) 
    {
        printf("Package \"%s\" not found\n", package_name);
        return 5;
    }

    char buffer[MAX_PACKAGE_FILE_SIZE];

    fread(buffer, MAX_PACKAGE_FILE_SIZE, 1, package_json_file);
    fclose(package_json_file);

    json_object *parsed_json; //The json object extracted from the package file
    json_object *name; //The name of the package
    json_object *dependencies; //Array of dependencies
    json_object *installation; //String of installation commands

    parsed_json = json_tokener_parse(buffer);

    //Gets the child objects from the main json object
    json_object_object_get_ex(parsed_json, "name", &name);
    json_object_object_get_ex(parsed_json, "dependencies", &dependencies);
    json_object_object_get_ex(parsed_json, "install", &installation);

    //If the package is in a wrong format. (it doesnt have a name or a installation instruction or dependencies object) the we inform the user about it
    if (name == NULL || dependencies == NULL || installation == NULL)
    {
        printf("Package Corrupted, Aborting...\n");
        return 6;
    }

    char* package_json_name = json_object_get_string(name); //This name might be different from the package name we search in the repo

    /*
    * Installing Package and dependencies 
    */

    printf("Package To Install: %s\n", package_json_name);

    size_t dependencies_length;

    dependencies_length = json_object_array_length(dependencies);

    //First we print the dependencies
    printf("Dependencies: \n");
    for (size_t i = 0; i < dependencies_length; i++) 
    {
        json_object *new_package = json_object_array_get_idx(dependencies, i);
        printf("    %s\n", (json_object_get_string(new_package)));
    }

    //Then we install every single dependency with their own dependencies recursevely
    printf("Installing Dependencies...\n");
    for (size_t i = 0; i < dependencies_length; i++) 
    {
        json_object *new_package = json_object_array_get_idx(dependencies, i);
        install_single_package(json_object_get_string(new_package));
    }

    //Then we install the main package
    printf("Installing Package: %s\n", package_json_name);
    system(json_object_get_string(installation));
}
