#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

#include "japml.h"
#include "handle.h" // definition for japml_handle_t
#include "log.h"
#include "error.h"
#include "colors.h"
#include "list.h"
#include "japmlcurses.h"

void terminal_init(japml_handle_t* handle)
{
    if (handle->use_curses)
    {
        curses_init();
    }
}

void japml_log(japml_handle_t* handle, japml_log_level_t log_level, char *message)
{
    japml_log_level_t handle_log_level = handle->log_level;

    if (log_level >= handle_log_level) // if throws error just cast to int
    {
        if (log_level < 2)
        {
            japml_log_normal_files(handle, log_level, message);
        }
        else
        {
            japml_log_error_files(handle, log_level, message);
        }

        if (handle->use_curses)
        {
            japml_ncurses_log(log_level, message, handle->use_colors);
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
            japml_debug_log(log_files->data, message, use_color);
        }
        else if (log_level == 1)
        {
            japml_info_log(log_files->data, message, use_color);
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
            japml_debug_log(log_files->data, message, use_color);
        }
        else if (log_level == 3)
        {
            japml_info_log(log_files->data, message, use_color);
        }

        log_files = japml_list_next(log_files);
    }
}

// * Debug Logging

void japml_debug_log(FILE *output, char *message, bool color)
{
    if (color) fprintf(output, ANSI_COLOR_YELLOW " ==> Debug: ");
    fprintf(output, "%s", message); 
    color_reset(output); 
}

// * Info Logging

void japml_info_log(FILE *output, char *message, bool color)
{
    if (color) fprintf(output, ANSI_COLOR_GREEN " ==> Inf: ");
    fprintf(output, "%s", message); 
    color_reset(output); 
}


// * Error Logging

void japml_error_log(FILE *output, char *message, bool color)
{
    if (color) fprintf(output, ANSI_COLOR_RED " ==> Err: ");
    fprintf(output, "%s", message);
    color_reset(output);
}


// * Critical Error Logging

void japml_critical_log(FILE *output, char *message, bool color)
{
    if (color) fprintf(output, ANSI_COLOR_MAGENTA " ==> Crit: ");
    fprintf(output, "%s", message);
    color_reset(output);
}

void color_reset(FILE *output) { fprintf(output, ANSI_COLOR_RESET); }