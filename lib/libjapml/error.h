#ifndef _ERROR_H_INCLUDED
#define _ERROR_H_INCLUDED

#include "japml.h"

enum _japml_error
{
    malloc_error = 3
};

/* Pritns an error to the log screen and exits if the error is critical */
void japml_throw_error(japml_handle_t *handle, japml_error_t error_code, char* message);

#endif