#ifndef _HANDLE_H_INCLUDED
#define _HANDLE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

#include "japml.h"

struct _japml_handle
{
    japml_db_local_t *local_db;
    japml_list_t     *remote_dbs; // list of japml_db_remote_t

    japml_log_level_t log_level;
    japml_list_t *log_files; // Place to write logs into (could be a text file or stdout for example)
    japml_list_t *error_log_files; // Place where errors would be written

    // TUI
    bool use_colors;
    bool use_curses;
    int progress_bar_window_height;
    int package_list_window_width;
};

#endif
