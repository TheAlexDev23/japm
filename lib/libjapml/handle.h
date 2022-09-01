#ifndef _HANDLE_H_INCLUDED
#define _HANDLE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include <curl/curl.h>

#include "japml.h"
#include "log.h"
#include "handle.h"
#include "db.h"
#include "list.h"
#include "error.h"
#include "japmlcurses.h"

struct _japml_handle
{
    // * DBs

    japml_db_local_t *local_db;
    japml_list_t     *remote_dbs; // list of japml_db_remote_t

    // * Logging

    /* If you want to format a message and use it in logging you can format this vairable */
    char* log_message;
    japml_log_level_t log_level; // The level of logging, only logs above it's level of importance would be shown
    japml_list_t *log_files; // Place to write logs into (could be a text file or stdout for example)
    japml_list_t *error_log_files; // Place where errors would be written

    // * CURL

    CURL* curl;
    CURLcode res;

    // * TUI

    bool use_colors;
    bool use_curses;

    // * Logging

    WINDOW *log_window;
    WINDOW *progress_window;
    WINDOW *package_list_window;

    japml_list_t* ncurses_log_buffer;
    int ncurses_log_buffer_count;
    int ncurses_log_buffer_length; // Max length of the buffer

    // * Errors
    /* Callbacks that will be called on error */
    japml_error_callbacks_t error_callbacks;
    /* True if JAPML should exit when the error passed to japml_throw_error is critical */
    bool exit_on_critical;

    // * Actions
    /* Action */
    japml_action_t* action;
};

#endif
