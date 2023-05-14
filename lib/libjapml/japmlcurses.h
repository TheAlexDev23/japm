#ifndef _JAPMLCURSES_H_INCLUDED
#define _JAPMLCURSES_H_INCLUDED

#include <ncurses.h>
#include <stdbool.h>

#include "japml.h"
#include "log.h"

// * Initialazation

// Initialize curses for JAPML
void curses_init(japml_handle_t* handle);

#define PROGRESS_BAR_WINDOW_HEIGHT 3
#define PACKAGE_LIST_WINDOW_WIDTH 50    

// * Log window
#define JAPML_NCURSES_LOG_BUFFER_MAX_LENGTH 100

/* Logs a message into the ncurses log screen */
void japml_ncurses_log(japml_handle_t* handle, japml_log_level_t log_level, char *message);
/* Re-draws log-screen's messages */
void japml_ncurses_log_win_refresh(japml_handle_t* handle);
/* Prints message into the log screen. Does not refresh. */
void japml_ncurses_log_win_print(japml_handle_t* handle, japml_log_message_t* message);
void japml_ncurses_free_log_buffer(japml_handle_t* handle);

// * Progress bar
/* Draws progress bar of length amnt. Doesn't check for overflows, it's better to use the set_lim and add functions of the pb API */
void japml_ncurses_draw_pb(japml_handle_t* handle, int amnt);
/* Re-draws progress bar with current progress */
void japml_ncurses_pb_refresh(japml_handle_t* handle);
/* Sets the limit of the progress bar */
void japml_ncurses_pb_set_lim(japml_handle_t* handle, int limit);
/* Advances progress by amnt */
void japml_ncurses_pb_add(japml_handle_t* handle, int amnt);

// * Packages window
/* Adds a package to package list */
void japml_ncurses_pl_add(japml_handle_t* handle, japml_package_t* package, japml_package_action_t action);
/* Re-draws package list window */
void japml_ncurses_pl_refresh(japml_handle_t* handle);

// * Dialogues

/* A yes / no dialgoe where yes is the default option */ 
bool japml_ncurses_Yn_dialogue(japml_handle_t* handle, char* message);
/* A yes / no dialogue where no is the default option */
bool japml_ncurses_yN_dialogue(japml_handle_t* handle, char* message);
/* A yes / no dialgue with no default option */
bool japml_ncurses_yn_dialogue(japml_handle_t* handle, char* message);

// * Colors

#define JAPML_CURSES_DEBUG_COLOR      1
#define JAPML_CURSES_INFO_COLOR       2
#define JAPML_CURSES_ERROR_COLOR      3
#define JAPML_CURSES_CRITICAL_COLOR   4
#define JAPML_CURSES_DO_NOT_USE_COLOR 100

#endif