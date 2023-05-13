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
/* Refreshes the log screen (by clearing and re-printing everything) and doesn't allow messages to overflow the screen */
void japml_ncurses_log_win_update(japml_handle_t* handle);
/* Prints the actual message into the log screen. It is not recomended to call it manually. */
void japml_ncurses_log_win_print(japml_handle_t* handle, japml_log_message_t* message);
/* Since the log buffer is a list of japml_log_message_t we need to first free whatever the data pointer is pointing at and then the list itself */
void japml_ncurses_free_log_buffer(japml_handle_t* handle);

// * Progress bar
/* Re-dras progress bar with current progress */
void japml_ncurses_pb_refresh(japml_handle_t* handle);
/* Sets the limit of the progress bar */
void japml_ncurses_pb_set_lim(japml_handle_t* handle, int limit);
/* Advances progress by 1 */
void japml_ncurses_pb_add(japml_handle_t* handle, int amnt);

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