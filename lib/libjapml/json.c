#include <stdio.h>
#include <json-c/json.h>

#include "japml.h"
#include "package.h"
#include "json.h"

int japml_parse_json_file(japml_handle_t* handle, char* file_location, japml_package_t** pkg)
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
	
	json_object *files;

    json_object *install;
    json_object *remove;

    if (!(
        json_object_object_get_ex(json_obj, "name", &name) && 

        json_object_object_get_ex(json_obj, "version", &version) && 
        json_object_object_get_ex(json_obj, "description", &description) &&

        json_object_object_get_ex(json_obj, "dependencies", &dependencies) &&
        
        json_object_object_get_ex(json_obj, "files", &files) &&

        json_object_object_get_ex(json_obj, "install", &install) &&
        json_object_object_get_ex(json_obj, "remove", &remove)
    ))
    {
        // Package corrupted
        
    }

    *pkg = malloc(sizeof(japml_package_t));

    (*pkg)->name = json_object_get_string(name);
    (*pkg)->name = json_object_get_string(name);
    (*pkg)->name = json_object_get_string(name);
    (*pkg)->name = json_object_get_string(name);
}
