#ifndef _JAPML_HANDLE_H_INCLUDED
#define _JAPML_HANDLE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include <curl/curl.h>
#include <sqlite3.h>

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
    japml_list_t *remote_dbs; // list of japml_db_remote_t

    // * Logging

    /* If you want to format a message and use it in logging you can format this vairable */
    char* log_message;
    japml_log_level_t log_level; // The level of logging, only logs above it's level of importance would be shown
    japml_list_t *log_files; // Place to write logs into (could be a text file or stdout for example)
    japml_list_t *error_log_files; // Place where errors would be written

    // * SQLite3
    sqlite3* sqlite;

    // * CURL

    CURL* curl;
    CURLcode res;

    // * TUI

    bool use_colors;
    bool use_ncurses;

    /* If true, in cases where JAPML asks the user for something, JAPML will automatically chose the default option (if available) */
    bool default_to_all;

    // * Ncurses windows
    
    WINDOW *log_window;
    WINDOW *progress_window;
    WINDOW *package_list_window;

    // * Ncurses logging
    /* Log buffer of ncurses log window */
    japml_list_t* ncurses_lb;
    /* Current index of ncurses_lb */
    int ncurses_lb_count;
    /* Max length of ncurses_lb */
    int ncurses_lb_length;

    // * Ncurses progress bar
    /* Progress in pb is between 0 and ncurses_pb_lim */
    int ncurses_pb_lim;
    /* Current pb progress */
    int ncurses_pb_progress;

    // * Ncurses package list
    /* Buffer of package list ncurses window */
    japml_list_t* ncurses_pl_buffer;
    /* Current index of ncurses_pl_buffer */
    int ncurses_pl_count;
    /* Length of ncurses_pl_buffer */
    int ncurses_pl_length;

    // * Errors
    /* True if JAPML should exit when the error passed to japml_throw_error is critical */
    bool exit_on_critical;

    // * Actions
    /* Action */
    japml_action_t* action;
};

#endif
