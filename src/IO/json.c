#include "json.h"

package parse_package_information(FILE *file)
{
    // Return a package struct with the information from the json file given as an argument using the json-c library

    char buffer[MAX_PACKAGE_METADATA_FILE_SIZE];
    fread(buffer, sizeof(char), MAX_PACKAGE_METADATA_FILE_SIZE, file);
    fclose(file);

    json_object *parsed_json; // The json object extracted from the package file

    json_object *name;
    json_object *version;
    json_object *description;

    json_object *dependencies;

    json_object *install;
    json_object *remove;
    json_object *update;

    parsed_json = json_tokener_parse(buffer);

    // Gets the child objects from the main json object
    json_object_object_get_ex(parsed_json, "name", &name);
    json_object_object_get_ex(parsed_json, "version", &version);
    json_object_object_get_ex(parsed_json, "description", &description);

    json_object_object_get_ex(parsed_json, "dependencies", &dependencies);

    json_object_object_get_ex(parsed_json, "install", &install);
    json_object_object_get_ex(parsed_json, "remove", &remove);
    json_object_object_get_ex(parsed_json, "update", &update);

    // Create a package struct
    package pkg;

    pkg.json_obj = parsed_json;

    pkg.name = name;
    pkg.version = version;

    pkg.description = description;
    pkg.dependencies = dependencies;

    pkg.install = install;
    pkg.remove = remove;
    pkg.update = update;

    return pkg;
}
