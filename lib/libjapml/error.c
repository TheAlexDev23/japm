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
        japml_log(handle, (int)error_code, message);
    }

    // The error is critical and we need to exit JAPML
    if (error_code == 3)
    {
        japml_ncurses_log(handle, Information, "Press any key to exit", handle->use_colors);
        getch();

        exit_japml(handle);

        exit((int)error_code);
    }
}

void japml_throw_malloc_error(japml_handle_t* handle) { _japml_throw_error(handle, malloc_error, "Could not allocate memory"); }
void japml_unkown_error(japml_handle_t* handle)       { _japml_throw_error(handle, unknown_error, "Unknown error"); }

void japml_throw_error(japml_handle_t* handle, japml_error_t error_code, char* message)
{
    japml_error_callbacks_t cbs = handle->error_callbacks;

    void (*japml_cb) (japml_handle_t*)    = NULL;
    void (*frontend_cb) (japml_handle_t*) = NULL;

    switch (error_code)
    {
        case malloc_error:
            japml_cb = &japml_throw_malloc_error;
            frontend_cb = cbs.cb_japml_malloc_error;
            break;
        default:
            japml_cb = &japml_unkown_error;
            frontend_cb = cbs.cb_japml_unkown_error;
            break;
    }

    (*japml_cb)(handle);
    if (frontend_cb)
    {
        (*frontend_cb)(handle);
    }
}