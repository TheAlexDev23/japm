#include <ncurses.h>
#include <string.h> // strlen
#include <unistd.h> // sleep

#include "japmlcurses.h"
#include "log.h"

void curses_init()
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

    log_window          = newwin(maxY - PROGRESS_BAR_WINDOW_HEIGHT, maxX - PACKAGE_LIST_WINDOW_WIDTH, 0, 0);
    progress_window     = newwin(PROGRESS_BAR_WINDOW_HEIGHT, maxX - PACKAGE_LIST_WINDOW_WIDTH, maxY - PROGRESS_BAR_WINDOW_HEIGHT, 0);
    package_list_window = newwin(maxY, PACKAGE_LIST_WINDOW_WIDTH, 0, maxX - PACKAGE_LIST_WINDOW_WIDTH);

    refresh();

    box(log_window,          ACS_VLINE, ACS_HLINE);
    box(progress_window,     ACS_VLINE, ACS_HLINE);
    box(package_list_window, ACS_VLINE, ACS_HLINE);

    wrefresh(log_window);
    wrefresh(progress_window);
    wrefresh(package_list_window);

    init_pair(JAPML_CURSES_DEBUG_COLOR,    COLOR_YELLOW,  COLOR_BLACK);
    init_pair(JAPML_CURSES_INFO_COLOR,     COLOR_GREEN,   COLOR_BLACK);
    init_pair(JAPML_CURSES_ERROR_COLOR,    COLOR_RED,     COLOR_BLACK);
    init_pair(JAPML_CURSES_CRITICAL_COLOR, COLOR_MAGENTA, COLOR_BLACK);

    japml_ncurses_log_buffer_length = getmaxx(log_window);
}

void japml_ncurses_log(japml_log_level_t log_level, char *message, bool use_color)
{
    if (japml_ncurses_log_buffer_count >= japml_ncurses_log_buffer_length)
    {
        // Move everithing back 1 spot
        for (int i = 0; i < japml_ncurses_log_buffer_count; i++)
        {
            japml_ncurses_log_buffer[i] = japml_ncurses_log_buffer[i + 1];
        }

        japml_ncurses_log_buffer_count--;
    }

    japml_ncurses_log_buffer[japml_ncurses_log_buffer_count] = message;
    if (use_color)
    {
        switch (log_level)
        {
            case 0:
                japml_ncurses_log_win_update(JAPML_CURSES_DEBUG_COLOR);
                break;
            case 1:
                japml_ncurses_log_win_update(JAPML_CURSES_INFO_COLOR);
                break;
            case 2:
                japml_ncurses_log_win_update(JAPML_CURSES_ERROR_COLOR);
                break;
            case 3:
                japml_ncurses_log_win_update(JAPML_CURSES_CRITICAL_COLOR);
                break;
        }
    }
    else
    {
        japml_ncurses_log_win_update(JAPML_CURSES_DO_NOT_USE_COLOR);
    }
}

// Update the log_window
void japml_ncurses_log_win_update(int color)
{
    wclear(log_window);
    box(log_window, ACS_VLINE, ACS_HLINE);

    refresh(); // idk just in case
    wrefresh(log_window);

    wmove(log_window, 1, 1);

    for (int i = 0; i < japml_ncurses_log_buffer_count; i++)
    {
        if (color != JAPML_CURSES_DO_NOT_USE_COLOR)
        {
            attron(COLOR_PAIR(color));
        }

        printw("%s", japml_ncurses_log_buffer[i]);
        wmove(log_window, getcury(log_window) + 1, getcurx(log_window));
       
        if (color != JAPML_CURSES_DO_NOT_USE_COLOR)
        {
            attroff(COLOR_PAIR(color));
        }
    }
}
