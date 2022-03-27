#include <stdio.h>
#include <stdlib.h>
#include <json-c/json.h>

#include "../Errors/errors.h"

#define MAX_PACKAGE_FILE_SIZE 2048

int _install_package(char*package_name);

struct package {
    char* name;
    char** dependencies;
    char* installation;
};

int
install_package(int argc, char**argv) 
{
    if (check_install_usage(argc, argv)) 
    {
        return 4;
    }

    return _install_package(argv[2]);
}

int
_install_package(char*package_name) 
{
    char*f_package_name = malloc(sizeof("../src/PackageManagment/") + sizeof(package_name) + sizeof(".json"));
    sprintf(f_package_name, "../src/PackageManagment/%s.json", package_name);
    FILE *package_json_file = fopen(f_package_name, "r");

    if (package_json_file == NULL) 
    {
        printf("Package \"%s\" not found\n", package_name);
        return 5;
    }

    char buffer[MAX_PACKAGE_FILE_SIZE];

    fread(buffer, MAX_PACKAGE_FILE_SIZE, 1, package_json_file);
    fclose(package_json_file);

    json_object *parsed_json;
    json_object *name;
    json_object *dependencies;
    json_object *installation;

    parsed_json = json_tokener_parse(buffer);

    json_object_object_get_ex(parsed_json, "name", &name);
    json_object_object_get_ex(parsed_json, "dependencies", &dependencies);
    json_object_object_get_ex(parsed_json, "install", &installation);

    if (name == NULL || dependencies == NULL || installation == NULL)
    {
        printf("Package Corrupted Aborting...\n");
        return 6;
    }

    printf("Package To Install: %s\n", json_object_get_string(name));

    size_t dependencies_length;

    dependencies_length = json_object_array_length(dependencies);

    printf("Dependencies: \n"); 
    for (size_t i = 0; i < dependencies_length; i++) 
    {
        json_object *new_package = json_object_array_get_idx(dependencies, i);
        printf("    %s\n", (json_object_get_string(new_package)));
    }

    printf("Installing Dependencies...\n");
    
    for (size_t i = 0; i < dependencies_length; i++) 
    {
        json_object *new_package = json_object_array_get_idx(dependencies, i);
        _install_package(json_object_get_string(new_package));
    }

    printf("Installing Package: %s\n", json_object_get_string(name));
    system(json_object_get_string(installation));
}