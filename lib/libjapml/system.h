#ifndef _JAPML_SYSTEM_H_INCLUDED
#define _JAPML_SYSTEM_H_INCLUDED

#include "japml.h"

/*
* Runs each string in instructions as a system command
* Returns -1 if command fails; 0 if all commands exited succesfully
*/
int japml_run_instructions(japml_list_t* instructions);

#endif