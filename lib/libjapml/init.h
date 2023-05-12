#ifndef _INIT_H_INCLUDED
#define _INIT_H_INCLUDED

#include "japml.h"
#include "handle.h"

japml_handle_t* japml_init_base();
japml_handle_t* japml_init_default();
japml_handle_t* japml_init_devel();
japml_handle_t* japml_init(japml_parse_parameters_t* parameters);
void japml_terminal_init(japml_handle_t* handle);

#endif