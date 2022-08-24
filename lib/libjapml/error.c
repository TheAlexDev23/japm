#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "error.h"
#include "japml.h" // japml_error_t
#include "handle.h"
#include "log.h"
#include "exit.h"
#include "japmlcurses.h"

void japml_throw_error(japml_handle_t *handle, japml_error_t error_code, char* message)
{
    if (message != NULL)
    {
        japml_log(handle, (int)error_code, message);
    }

    exit_japml(handle);

    japml_ncurses_log(handle, Information, "Press any key to exit", handle->use_colors);
    getch();

    exit((int)error_code);
}