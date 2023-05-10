#ifndef _JSON_H_INCLUDED
#define _JSON_H_INCLUDED

#include <json-c/json.h>
#include <stdio.h>

#include "japml.h"
#include "package.h"
#include "handle.h"

japml_package_t* japml_parse_json_file(japml_handle_t* handle, char* file_location);

#endif