#ifndef _JAPML_SYSTEM_H_INCLUDED
#define _JAPML_SYSTEM_H_INCLUDED

#include "japml.h"

#define JAPML_SYSTEM_OUT_STDIO "/tmp/japml/outstd"
#define JAPML_SYSTEM_OUT_ERR "/tmp/japml/outerr"
#define JAPML_SYSTEM_OUT_LOG_MAX 500
/*
* Runs each string in instructions as a system command
* Returns -1 if command fails; 0 if all commands exited succesfully
*/
int japml_run_instructions_silent(japml_list_t* instructions, char* dir);
int japml_run_instructions_with_log(japml_handle_t* handle, japml_list_t* instructions, char* dir);

#endif