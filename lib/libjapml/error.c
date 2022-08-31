#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "error.h"
#include "japml.h" // japml_error_t
#include "handle.h"
#include "log.h"
#include "exit.h"
#include "japmlcurses.h"

void _japml_throw_error(japml_handle_t *handle, japml_error_t error_code, char* message)
{
    if (message != NULL)
    {
        if (error_code < custom_error_error)
        {
            japml_log(handle, Information, message);
        }
        else if (error_code < custom_error_critical)
        {
            japml_log(handle, Error, message);
        }
        else
        {
            japml_log(handle, Critical, message);
        }
    }

    // The error is critical and we need to exit JAPML
    if (error_code >= custom_error_critical && handle->exit_on_critical)
    {
        japml_ncurses_log(handle, Information, "Press any key to exit", handle->use_colors);
        getch();

        exit_japml(handle);

        exit((int)error_code);
    }
}

void japml_throw_malloc_error(japml_handle_t* handle, char*msg)
{ 
    _japml_throw_error(handle, malloc_error, msg != NULL ? msg : "Could not allocate memory");
}

void japml_unkown_error(japml_handle_t* handle, char*msg)
{
     _japml_throw_error(handle, unknown_error, msg != NULL ? msg : "Unknown error"); 
}

void japml_throw_install_error(japml_handle_t* handle, char*msg)
{ 
    _japml_throw_error(handle, install_error, msg != NULL ? msg : "Package install failed. One of the install comands returned failure");
}

void japml_throw_dependency_break_error(japml_handle_t* handle, char*msg)
{ 
    _japml_throw_error(handle, dependency_break_error, msg != NULL ? msg : "Dependency break error"); 
}

void japml_throw_error(japml_handle_t* handle, japml_error_t error_code, char* message)
{
    japml_error_callbacks_t cbs = handle->error_callbacks;

    void (*japml_cb) (japml_handle_t*, char*) = NULL;
    void (*frontend_cb) (japml_handle_t*) = NULL;

    switch (error_code)
    {
        case malloc_error:
            japml_cb = &japml_throw_malloc_error;
            frontend_cb = cbs.cb_japml_malloc_error;
            break;
        case install_error:
            japml_cb = &japml_throw_install_error;
            frontend_cb = cbs.cb_japml_install_error;
            break;
        case dependency_break_error:
            japml_cb = &japml_throw_dependency_break_error;
            frontend_cb = cbs.cb_japml_dependency_break_error;
        default:
            japml_cb = &japml_unkown_error;
            frontend_cb = cbs.cb_japml_unkown_error;
            break;
    }

    (*japml_cb)(handle, message);
    if (frontend_cb)
    {
        (*frontend_cb)(handle);
    }
}