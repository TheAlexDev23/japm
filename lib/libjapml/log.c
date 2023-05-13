#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

#include "japml.h"
#include "handle.h" // definition for japml_handle_t
#include "japmlcurses.h"
#include "log.h"
#include "error.h"
#include "colors.h"
#include "list.h"


void japml_log(japml_handle_t* handle, japml_log_level_t log_level, char *message)
{
    japml_log_level_t handle_log_level = handle->log_level;

    if (log_level >= handle_log_level)
    {
        if (log_level < 2)
        {
            japml_log_normal_files(handle, log_level, message);
        }
        else
        {
            japml_log_error_files(handle, log_level, message);
        }

        if (handle->use_ncurses)
        {
            japml_ncurses_log(handle, log_level, message);
        }
    }
}

void japml_log_normal_files(japml_handle_t* handle, japml_log_level_t log_level, char *message)
{
    japml_list_t* log_files = handle->log_files;
    bool use_color = handle->use_colors;

    while (log_files)
    {
        if (log_level == 0)
        {
            japml_debug_log((FILE*)(log_files->data), message, use_color);
        }
        else if (log_level == 1)
        {
            japml_info_log((FILE*)(log_files->data), message, use_color);
        }

        log_files = japml_list_next(log_files);
    }
}

void japml_log_error_files(japml_handle_t* handle, japml_log_level_t log_level, char *message)
{
    japml_list_t *log_files = handle->error_log_files;
    bool use_color = handle->use_colors;

    while (log_files)
    {
        if (log_level == 2)
        {
            japml_debug_log((FILE*)(log_files->data), message, use_color);
        }
        else if (log_level == 3)
        {
            japml_info_log((FILE*)(log_files->data), message, use_color);
        }

        log_files = japml_list_next(log_files);
    }
}

// * Debug Logging

void japml_debug_log(FILE *output, char *message, bool color)
{
    if (color) fprintf(output, ANSI_COLOR_YELLOW);
    fprintf(output, " ==> Debug: ");
    fprintf(output, "%s\n", message); 
    japml_color_reset(output); 
}

// * Info Logging

void japml_info_log(FILE *output, char *message, bool color)
{
    if (color) fprintf(output, ANSI_COLOR_GREEN);
    fprintf(output, " ==> Inf: ");
    fprintf(output, "%s\n", message); 
    japml_color_reset(output); 
}


// * Error Logging

void japml_error_log(FILE *output, char *message, bool color)
{
    if (color) fprintf(output, ANSI_COLOR_RED); 
    fprintf(output, " ==> Err: ");
    fprintf(output, "%s\n", message);
    japml_color_reset(output);
}


// * Critical Error Logging

void japml_critical_log(FILE *output, char *message, bool color)
{
    if (color) fprintf(output, ANSI_COLOR_MAGENTA);
    fprintf(output, " ==> Crit: ");
    fprintf(output, "%s\n", message);
    japml_color_reset(output);
}

void japml_color_reset(FILE *output) { fprintf(output, ANSI_COLOR_RESET); }