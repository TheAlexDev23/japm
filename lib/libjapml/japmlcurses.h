#ifndef _JAPMLCURSES_H_INCLUDED
#define _JAPMLCURSES_H_INCLUDED

#include <ncurses.h>
#include <stdbool.h>

#include "japml.h"
#include "log.h"

// * Initialazation

// Initialize curses for JAPM
void curses_init(japml_handle_t* handle);

#define PROGRESS_BAR_WINDOW_HEIGHT 3
#define PACKAGE_LIST_WINDOW_WIDTH 50    

// * Log window
#define JAPML_NCURSES_LOG_BUFFER_MAX_LENGTH 100

void japml_ncurses_log(japml_handle_t* handle, japml_log_level_t log_level, char *message, bool use_color);
void japml_ncurses_log_win_update(japml_handle_t* handle);
void japml_ncurses_log_win_print(japml_handle_t* handle, japml_log_message_t* message);

// * Colors

#define JAPML_CURSES_DEBUG_COLOR      1
#define JAPML_CURSES_INFO_COLOR       2
#define JAPML_CURSES_ERROR_COLOR      3
#define JAPML_CURSES_CRITICAL_COLOR   4
#define JAPML_CURSES_DO_NOT_USE_COLOR 100

#endif