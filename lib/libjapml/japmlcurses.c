#include <ncurses.h>
#include <string.h> // strlen
#include <unistd.h> // sleep

#include "japml.h"
#include "handle.h"
#include "japmlcurses.h"
#include "log.h"

void curses_init(japml_handle_t *handle)
{
    initscr();

    int maxX, maxY;

    getmaxyx(stdscr, maxY, maxX);

    char *msg_japm = "JAPM version 1.0.0";
    char *msg_japml = "JAPML version 1.0.0";
    char *msg_creator = "By TheAlexDev23 (https://github.com/thealexdev23)";

    mvprintw(maxY / 2 - 1, (maxY-strlen(msg_japm)) / 2, "%s", msg_japm);
    mvprintw(maxY / 2, (maxY-strlen(msg_japml)) / 2, "%s", msg_japml);
    mvprintw(maxY / 2 + 1, (maxY-strlen(msg_creator)) / 2, "%s", msg_creator);

    sleep(2);

    clear();

    handle->log_window          = newwin(maxY - PROGRESS_BAR_WINDOW_HEIGHT, maxX - PACKAGE_LIST_WINDOW_WIDTH, 0, 0);
    handle->progress_window     = newwin(PROGRESS_BAR_WINDOW_HEIGHT, maxX - PACKAGE_LIST_WINDOW_WIDTH, maxY - PROGRESS_BAR_WINDOW_HEIGHT, 0);
    handle->package_list_window = newwin(maxY, PACKAGE_LIST_WINDOW_WIDTH, 0, maxX - PACKAGE_LIST_WINDOW_WIDTH);

    refresh();

    box(handle->log_window,          ACS_VLINE, ACS_HLINE);
    box(handle->progress_window,     ACS_VLINE, ACS_HLINE);
    box(handle->package_list_window, ACS_VLINE, ACS_HLINE);

    wrefresh(handle->log_window);
    wrefresh(handle->progress_window);
    wrefresh(handle->package_list_window);

    init_pair(JAPML_CURSES_DEBUG_COLOR,    COLOR_YELLOW,  COLOR_BLACK);
    init_pair(JAPML_CURSES_INFO_COLOR,     COLOR_GREEN,   COLOR_BLACK);
    init_pair(JAPML_CURSES_ERROR_COLOR,    COLOR_RED,     COLOR_BLACK);
    init_pair(JAPML_CURSES_CRITICAL_COLOR, COLOR_MAGENTA, COLOR_BLACK);

    handle->ncurses_log_buffer_length = getmaxx(handle->log_window) - 5;
}

void japml_ncurses_log(japml_handle_t* handle, japml_log_level_t log_level, char *message, bool use_color)
{
    if (handle->ncurses_log_buffer_count >= handle->ncurses_log_buffer_length)
    {
        // Move everithing back 1 spot
        for (int i = 0; i < handle->ncurses_log_buffer_count; i++)
        {
            handle->ncurses_log_buffer[i] = handle->ncurses_log_buffer[i + 1];
        }

        handle->ncurses_log_buffer_count--;
    }

    handle->ncurses_log_buffer[handle->ncurses_log_buffer_count] = message;
    if (use_color)
    {
        switch (log_level)
        {
            case 0:
                japml_ncurses_log_win_update(handle, JAPML_CURSES_DEBUG_COLOR);
                break;
            case 1:
                japml_ncurses_log_win_update(handle, JAPML_CURSES_INFO_COLOR);
                break;
            case 2:
                japml_ncurses_log_win_update(handle, JAPML_CURSES_ERROR_COLOR);
                break;
            case 3:
                japml_ncurses_log_win_update(handle, JAPML_CURSES_CRITICAL_COLOR);
                break;
        }
    }
    else
    {
        japml_ncurses_log_win_update(handle, JAPML_CURSES_DO_NOT_USE_COLOR);
    }
}

// Update the log_window
void japml_ncurses_log_win_update(japml_handle_t *handle, int color)
{
    wclear(handle->log_window);
    box(handle->log_window, ACS_VLINE, ACS_HLINE);

    refresh(); // idk just in case
    wrefresh(handle->log_window);

    wmove(handle->log_window, 1, 1);

    for (int i = 0; i < handle->ncurses_log_buffer_count; i++)
    {
        if (color != JAPML_CURSES_DO_NOT_USE_COLOR)
        {
            attron(COLOR_PAIR(color));
        }

        printw("%s", handle->ncurses_log_buffer[i]);
        wmove(handle->log_window, getcury(handle->log_window) + 1, getcurx(handle->log_window));
       
        if (color != JAPML_CURSES_DO_NOT_USE_COLOR)
        {
            attroff(COLOR_PAIR(color));
        }
    }
}
