#ifndef _INIT_H_INCLUDED
#define _INIT_H_INCLUDED

#include "japml.h"
#include "handle.h"

japml_handle_t* japml_init_default(int argc, char* argv[]);
japml_handle_t* japml_init_devel(int argc, char* argv[]);
japml_handle_t* japml_init(int argc, char* argv[]);
void terminal_init(japml_handle_t* handle);

#endif