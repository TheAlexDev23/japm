#include <json-c/json.h>
#include <stdio.h>

#define MAX_PACKAGE_METADATA_FILE_SIZE 131072

typedef struct package package;

struct package{
    json_object *json_obj;
    
    json_object *name;
    json_object *version;
    json_object *description;

    json_object *dependencies;

    json_object *install;
    json_object *remove;
    json_object *update;
};

package parse_package_information(FILE *file);
