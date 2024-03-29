#ifndef _JAPML_JSON_H_INCLUDED
#define _JAPML_JSON_H_INCLUDED

#include <json-c/json.h>
#include <stdio.h>

#include "japml.h"
#include "package.h"
#include "handle.h"

/* Allocates memory for and creates a japml package from the json file provided */
japml_package_t* japml_json_parse_file(japml_handle_t* handle, char* file_location);
/* A json list is converted into a japml list */
japml_list_t *japml_json_to_list(json_object *obj);
/* 
* Returns a list of japml_package_file_t from obj.
* Obj is an array
*/
japml_list_t* japml_json_to_pkg_file(json_object *obj);
/* Returns string from json obj */
char* japml_json_obj_to_string(json_object* obj);

#endif