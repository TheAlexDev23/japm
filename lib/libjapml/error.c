#include <stdlib.h>

#include "error.h"
#include "japml.h" // japml_error_t
#include "log.h"
#include "exit.h"

void japml_throw_error(japml_handle_t *handle, japml_error_t error_code, char* message)
{
    if (message != NULL)
    {
        japml_log(handle, (int)error_code, message);
    }

    exit_japml(handle);
    exit((int)error_code);
}