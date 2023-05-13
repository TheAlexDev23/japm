#ifndef _JAPML_ERROR_H_INCLUDED
#define _JAPML_ERROR_H_INCLUDED

#include "japml.h"

enum _japml_error
{
    custom_error_warning,

    custom_error_error,
    action_not_accepted_error,
    not_implemented_error,

    custom_error_critical,
    sql_command_fail_error,
    package_not_found_error,
    dependency_break_error,
    package_corrupted_error,
    install_error,
    malloc_error,
    unknown_error
};

/* Log error. Exit if required by handle and error level. */
void japml_throw_error(japml_handle_t *handle, japml_error_t error_code, char* msg);

#endif