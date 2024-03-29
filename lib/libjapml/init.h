#ifndef _JAPML_INIT_H_INCLUDED
#define _JAPML_INIT_H_INCLUDED

#include "japml.h"
#include "handle.h"

japml_handle_t* japml_init_base();
japml_handle_t* japml_init_default();
japml_handle_t* japml_init_devel();
/* Runs base init and overides handle default values with parameters */
japml_handle_t* japml_init(japml_parse_parameters_t* parameters);
void japml_init_terminal(japml_handle_t* handle);

#endif