#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

#include "japml.h"
#include "log.h"
#include "error.h"
#include "colors.h"
#include "list.h"

WINDOW *log_window;
WINDOW *progress_window;
WINDOW *package_list_window;

void terminal_init(japml_handle_t* handle)
{
    if (handle->use_curses)
    {
        initscr();

        int maxX, maxY;

        getmaxyx(stdscr, maxY, maxX);

        log_window          = newwin(maxY - handle->progress_bar_window_height, maxX - handle->package_list_window_width, 0, 0);
        progress_window     = newwin(handle->progress_bar_window_height, maxX - handle->package_list_window_width, maxY - handle->progress_bar_window_height, 0);
        package_list_window = newwin(maxY, handle->package_list_window_width, 0, maxX - handle->package_list_window_width);

        refresh();

        box(log_window,          ACS_VLINE, ACS_HLINE);
        box(progress_window,     ACS_VLINE, ACS_HLINE);
        box(package_list_window, ACS_VLINE, ACS_HLINE);

        wrefresh(log_window);
        wrefresh(progress_window);
        wrefresh(package_list_window);
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
    }
}

void japml_log_normal_files(japml_handle_t* handle, japml_log_level_t log_level, char *message)
{
    jampl_list_t log_files = japml_handle_t->log_files;

    while (log_files)
    {
        if (log_level == 0)
        {
            japml_debug_log(log_files->data, message);
        }
        else if (log_level == 1)
        {
            japml_info_log(log_files->data, message);
        }

        log_files = japml_list_next(log_files);
    }
}

void japml_log_error_files(japml_handle_t* hanlde, japml_log_level_t log_level, char *message)
{
    jampl_list_t log_files = japml_handle_t->error_log_files;

    while (log_files)
    {
        if (log_level == 2)
        {
            japml_debug_log(log_files->data, message);
        }
        else if (log_level == 3)
        {
            japml_info_log(log_files->data, message);
        }

        log_files = japml_list_next(log_files);
    }
}

// * Debug Logging

void japml_debug_log(FILE *output, char *message)
{
    fprintf(output, ANSI_COLOR_YELLOW " ==> Debug: ");
    fprintf(output, message); 
    color_reset(output); 
}

// * Info Logging

void japml_info_log(FILE *output, char *message)
{
    fprintf(output, ANSI_COLOR_GREEN " ==> Inf: ");
    fprintf(output, message); 
    color_reset(output); 
}


// * Error Logging

void japml_error_log(FILE *output, char *message)
{
    fprintf(output, ANSI_COLOR_RED " ==> Err: ");
    fprintf(output, message);
    color_reset(output);
}


// * Critical Error Logging

void japml_critical_log(FILE *output, char *message)
{
    fprintf(output, ANSI_COLOR_MAGENTA " ==> Crit: ");
    fprintf(output, message);
    color_reset(output);
}

void color_reset(FILE *output) { fprintf(output, ANSI_COLOR_RESET); }