#include <stdio.h>
#include <json-c/json.h>

#include "japml.h"
#include "list.h"
#include "package.h"
#include "json.h"

japml_package_t* japml_parse_json_file(japml_handle_t* handle, char* file_location)
{
    // Open the file
    FILE *fp = fopen(file_location, "r");

    // Get the file size
    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char buffer[file_size];
    fread(buffer, file_size, 1, fp);

    json_object *json_obj = json_tokener_parse(buffer);

    json_object *name;
    json_object *version;
    json_object *description;

    json_object *dependencies;
    json_object *build_dependencies;
	
	json_object *files;

    json_object *install;
    json_object *remove;

    if (!(
        json_object_object_get_ex(json_obj, "name", &name) && 

        json_object_object_get_ex(json_obj, "version", &version) && 
        json_object_object_get_ex(json_obj, "description", &description) &&

        json_object_object_get_ex(json_obj, "dependencies", &dependencies) &&
        json_object_object_get_ex(json_obj, "build dependencies", &build_dependencies) &&

        json_object_object_get_ex(json_obj, "files", &files) &&

        json_object_object_get_ex(json_obj, "install", &install) &&
        json_object_object_get_ex(json_obj, "remove", &remove)
    ))
    {
        // Package corrupted
        sprintf(handle->log_message, "Package (file location: %s) json file is corrupted. Some of the fields are missing.", file_location);
        japml_throw_error(handle, package_corrupted_error, handle->log_message);
    }

    japml_package_t *pkg = malloc(sizeof(japml_package_t));

    pkg->name = (char*)json_object_get_string(name);
    pkg->version = (char*)json_object_get_string(version);
    pkg->description = (char*)json_object_get_string(description);

    for (int i = 0; i < json_object_array_length(dependencies); i++)
    {
        json_object* dep = json_object_array_get_idx(dependencies, i);
        japml_list_add(handle, &pkg->deps, (char*)json_object_get_string(dep));
    }

    for (int i = 0; i < json_object_array_length(build_dependencies); i++)
    {
        json_object* dep = json_object_array_get_idx(build_dependencies, i);
        japml_list_add(handle, &pkg->build_deps, (char*)json_object_get_string(dep));
    }

    for (int i = 0; i < json_object_array_length(files); i++)
    {
        json_object* file = json_object_array_get_idx(files, i);
        japml_list_add(handle, &pkg->files, (char*)json_object_get_string(file));
    }

    for (int i = 0; i < json_object_array_length(install); i++)
    {
        json_object* cmd = json_object_array_get_idx(install, i);
        japml_list_add(handle, &pkg->install, (char*)json_object_get_string(cmd));
    }

    for (int i = 0; i < json_object_array_length(remove); i++)
    {
        json_object* cmd = json_object_array_get_idx(remove, i);
        japml_list_add(handle, &pkg->remove, (char*)json_object_get_string(cmd));
    }
}
