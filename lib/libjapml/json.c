#include <sys/stat.h>
#include <stdio.h>
#include <json-c/json.h>

#include "japml.h"
#include "list.h"
#include "package.h"
#include "json.h"

/* A json list is converted into a japml list */
japml_list_t *japml_json_to_list(japml_handle_t *handle, json_object *obj)
{
    japml_list_t *list = NULL;

    for (int i = 0; i < json_object_array_length(obj); i++)
    {
        json_object *item = json_object_array_get_idx(obj, i);
        japml_list_add(handle, &list, (char *)json_object_get_string(item));
    }

    return list;
}

/* Allocates memory for and creates a japml package from the json file provided */
japml_package_t *japml_parse_json_file(japml_handle_t *handle, char *file_location)
{
    // Open the file
    FILE *fp = fopen(file_location, "r");

    if (!fp)
    {
        japml_throw_error(handle, custom_error_error, "Cannot open json file");
        return NULL;
    }

    // Get the file size
    struct stat st;
    if (stat(file_location, &st))
    {
        japml_throw_error(handle, custom_error_error, "Cannot read file stat");
        return NULL;
    }

    int file_size = st.st_size;

    sprintf(handle->log_message, "%i", file_size);
    japml_log(handle, Information, handle->log_message);

    char *buffer = malloc(file_size + 1);
    buffer[file_size] = '\0';

    fread(buffer, 1, file_size, fp);

    json_object *json_obj = json_tokener_parse(buffer);

    json_object *name;
    json_object *version;
    json_object *description;

    json_object *dependencies;
    json_object *build_dependencies;

    json_object *files;

    json_object *pre_install;
    json_object *install;
    json_object *post_install;

    json_object *remove;

    if (!(
            json_object_object_get_ex(json_obj, "name", &name) &&

            json_object_object_get_ex(json_obj, "version", &version) &&
            json_object_object_get_ex(json_obj, "description", &description) &&

            json_object_object_get_ex(json_obj, "dependencies", &dependencies) &&
            json_object_object_get_ex(json_obj, "build dependencies", &build_dependencies) &&

            json_object_object_get_ex(json_obj, "files", &files) &&

            json_object_object_get_ex(json_obj, "pre install", &pre_install) &&
            json_object_object_get_ex(json_obj, "install", &install) &&
            json_object_object_get_ex(json_obj, "post install", &post_install) &&

            json_object_object_get_ex(json_obj, "remove", &remove)))
    {
        // Package corrupted
        sprintf(handle->log_message, "Package (file location: %s) json file is corrupted. Some of the fields are missing. \n %s", file_location, buffer);
        japml_throw_error(handle, package_corrupted_error, handle->log_message);
        free(buffer);
    }

    free(buffer);

    japml_package_t *pkg = japml_create_empty_package();

    pkg->name = (char *)json_object_get_string(name);
    pkg->version = (char *)json_object_get_string(version);
    pkg->description = (char *)json_object_get_string(description);

    pkg->deps = japml_json_to_list(handle, dependencies);
    pkg->build_deps = japml_json_to_list(handle, build_dependencies);

    pkg->pre_install = japml_json_to_list(handle, pre_install);
    pkg->install = japml_json_to_list(handle, install);
    pkg->post_install = japml_json_to_list(handle, post_install);

    pkg->remove = japml_json_to_list(handle, remove);

    return pkg;
}
