#ifndef _JAPMLCURSES_H_INCLUDED
#define _JAPMLCURSES_H_INCLUDED

#include <ncruses.h>

WINDOW *log_window;
WINDOW *progress_window;
WINDOW *package_list_window;

#define PROGRESS_BAR_WINDOW_HEIGHT 3
#define PACKAGE_LIST_WINDOW_WIDTH 50    

#define JAPML_NCURSES_LOG_BUFFER_LENGTH (getmaxy(log_window) - 2)

// * Colors

#define JAPML_CURSES_DEBUG_COLOR    1
#define JAPML_CURSES_INFO_COLOR     2
#define JAPML_CURSES_ERROR_COLOR    3
#define JAPML_CURSES_CRITICAL_COLOR 4


void curses_init();
void japml_ncurses_log(japml_log_level_t log_level, char *message)
void japml_ncurses_log_win_update();

#endif