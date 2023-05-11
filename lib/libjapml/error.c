#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "error.h"
#include "japml.h" // japml_error_t
#include "handle.h"
#include "log.h"
#include "exit.h"
#include "japmlcurses.h"

void japml_throw_error(japml_handle_t *handle, japml_error_t error_code, char* msg)
{
    if (msg != NULL)
    {
        if (error_code < custom_error_error)
        {
            japml_log(handle, Information, msg);
        }
        else if (error_code < custom_error_critical)
        {
            japml_log(handle, Error, msg);
        }
        else
        {
            japml_log(handle, Critical, msg);
        }
    }

    // The error is critical and we need to exit JAPML
    if (error_code >= custom_error_critical && handle->exit_on_critical)
    {
        japml_ncurses_log(handle, Information, "Press any key to exit");
        getch();

        japml_exit(handle);

        exit((int)error_code);
    }
}