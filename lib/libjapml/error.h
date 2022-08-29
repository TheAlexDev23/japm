#ifndef _ERROR_H_INCLUDED
#define _ERROR_H_INCLUDED

#include "japml.h"

enum _japml_error
{
    /* If there's a need to call a 'warning' error, but the type needed doesn't exist
     in japml_error_t call japml_throw_error if error_code as custom_error_warning */
    custom_error_warning = 1,
    /* If there's a need to call a 'error' error, but the type needed doesn't exist
     in japml_error_t call japml_throw_error if error_code as custom_error_warning */
    custom_error_error = 2,
    /* If there's a need to call a 'critical' error, but the type needed doesn't exist
     in japml_error_t call japml_throw_error if error_code as custom_error_warning */
    custom_error_critical = 3,
    malloc_error = 3,
    unknown_error = 3
};

struct _japml_error_callbacks 
{
    void (*cb_japml_unkown_error) (japml_handle_t*);
    void (*cb_japml_malloc_error) (japml_handle_t*);
};

/* Prints an error to the log screen and exits if the error is critical */
void japml_throw_error(japml_handle_t *handle, japml_error_t error_code, char* message);
/* Called inside JAPML by JAPML*/
void _japml_throw_error(japml_handle_t* handle, japml_error_t error_code, char* message);

void japml_unkown_error(japml_handle_t* handle);
void japml_throw_malloc_error(japml_handle_t* handle);

#endif