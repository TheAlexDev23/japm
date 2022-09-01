#ifndef _ERROR_H_INCLUDED
#define _ERROR_H_INCLUDED

#include "japml.h"

enum _japml_error
{
    /* If there's a need to call a 'warning' error, but the type needed doesn't exist
     in japml_error_t call japml_throw_error with error_code as custom_error_warning */
    custom_error_warning,
    /* If there's a need to call a 'error' error, but the type needed doesn't exist
     in japml_error_t call japml_throw_error with error_code as custom_error_warning */
    custom_error_error,
    /* If there's a need to call a 'critical' error, but the type needed doesn't exist
     in japml_error_t call japml_throw_error with error_code as custom_error_warning */
    custom_error_critical,
    /* Removing the package breaks a depenency */
    dependency_break_error,
    /* Errow while running system commands used to install package */
    package_corrupted_error,
    install_error,
    /* Error while allocating memory with malloc */
    malloc_error,
    unknown_error
};

struct _japml_error_callbacks 
{
    void (*cb_japml_unkown_error)            (japml_handle_t*);
    void (*cb_japml_dependency_break_error)  (japml_handle_t*);
    void (*cb_japml_install_error)           (japml_handle_t*);
    void (*cb_japml_malloc_error)            (japml_handle_t*);
    void (*cb_japml_package_corrupted_error) (japml_handle_t*);
};

/* Prints an error to the log screen and exits if the error is critical */
void japml_throw_error(japml_handle_t *handle, japml_error_t error_code, char* msg);
void _japml_throw_error(japml_handle_t* handle, japml_error_t error_code, char* message);

void japml_unkown_error(japml_handle_t* handle, char*msg);
void japml_throw_malloc_error(japml_handle_t* handle, char*msg);
void japml_throw_install_error(japml_handle_t* handle, char*msg);
void japml_throw_dependency_break_error(japml_handle_t* handle, char*msg);

#endif