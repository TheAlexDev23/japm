#ifndef _ERROR_H_INCLUDED
#define _ERROR_H_INCLUDED

#include "japml.h"

enum _japml_error
{
    malloc_error = 2
};

void japml_throw_error(japml_error_t error_code, char* message); // message can be null if no message needs to be provided

#endif