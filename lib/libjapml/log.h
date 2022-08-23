#ifndef _LOG_H_INCLUDED
#define _LOG_H_INCLUDED

#include "japml.h"

enum _japml_log_level
{
    Debug = 0,
    Information = 1,
    Error = 2,
    Ciritical = 3
};

void japml_log(japml_handle_t* handle, japml_log_level_t log_level, char *message);

#endif
