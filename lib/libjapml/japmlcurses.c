#include <ncurses.h>
#include <string.h> // strlen
#include <unistd.h> // sleep
#include <stdio.h>

#include "japml.h"
#include "handle.h"
#include "error.h"
#include "japmlcurses.h"
#include "log.h"

void curses_init(japml_handle_t *handle)
{
    initscr();
    
    if (handle->use_colors)
    {
        start_color();

        init_pair(JAPML_CURSES_DEBUG_COLOR,    COLOR_YELLOW,  COLOR_BLACK);
        init_pair(JAPML_CURSES_INFO_COLOR,     COLOR_GREEN,   COLOR_BLACK);
        init_pair(JAPML_CURSES_ERROR_COLOR,    COLOR_RED,     COLOR_BLACK);
        init_pair(JAPML_CURSES_CRITICAL_COLOR, COLOR_MAGENTA, COLOR_BLACK);
    }

    int maxX, maxY;

    getmaxyx(stdscr, maxY, maxX);

    // Splash screen
    char *msg_japm    = "JAPM version 1.0.0";
    char *msg_japml   = "JAPML version 1.0.0";
    char *msg_creator = "By TheAlexDev23 (https://github.com/thealexdev23)";

    mvprintw(maxY / 2 - 1, (maxX-strlen(msg_japm)) / 2, "%s", msg_japm);
    mvprintw(maxY / 2, (maxX-strlen(msg_japml)) / 2, "%s", msg_japml);
    mvprintw(maxY / 2 + 1, (maxX-strlen(msg_creator)) / 2, "%s", msg_creator);

    refresh();

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

}

void japml_ncurses_log(japml_handle_t* handle, japml_log_level_t log_level, char *message, bool use_color)
{
    handle->ncurses_log_buffer_length = getmaxy(handle->log_window) - 5;
    japml_log_message_t* message_struct = malloc(sizeof(japml_log_message_t));
    
    if (!message_struct)
    {
        char error_message[60]; // I dont't know how big the message would be
        sprintf(error_message, "Could not allocate memory of size %zu\n", sizeof(japml_list_t));
        japml_throw_error(handle, malloc_error, error_message);
    }

    message_struct->log_level = log_level;
    message_struct->message = message;

    FILE *f = fopen("haha.txt", "a");
    fprintf(f, "1\n");
    fclose(f);

    japml_list_add(handle, handle->ncurses_log_buffer, message_struct);
    if (handle->ncurses_log_buffer == NULL)
    {
        f = fopen("haha.txt", "a");
        fprintf(f, "UMMM\n");
        fclose(f);
    }
    else if (handle->ncurses_log_buffer->data == NULL)
    {
        f = fopen("haha.txt", "a");
        fprintf(f, "UMMM2\n");
        fclose(f);
    }

    handle->ncurses_log_buffer_count++;

    japml_ncurses_log_win_update(handle);
    f = fopen("haha.txt", "a");
    fprintf(f, "3\n");
    fclose(f);
}

// Update the log_window
void japml_ncurses_log_win_update(japml_handle_t *handle)
{
    wclear(handle->log_window);
    wmove(handle->log_window, 1, 1);

    if (handle->ncurses_log_buffer_count > handle->ncurses_log_buffer_length)
    {
        for (int i = handle->ncurses_log_buffer_count - handle->ncurses_log_buffer_length; i < handle->ncurses_log_buffer_count; i++)
        {
            japml_ncurses_log_win_print(handle, (japml_log_message_t*)(japml_list_get_element(handle->ncurses_log_buffer, i)->data));
        }
    }
    else
    {
        for (int i = 0; i < handle->ncurses_log_buffer_count; i++)
        {
            japml_ncurses_log_win_print(handle, (japml_log_message_t*)(japml_list_get_element(handle->ncurses_log_buffer, i)->data));
        }
    }

    box(handle->log_window, ACS_VLINE, ACS_HLINE);
    refresh();
    wrefresh(handle->log_window);
}

void japml_ncurses_log_win_print(japml_handle_t* handle, japml_log_message_t* message)
{
    if (!message)
    {
        return;
    }

    int color;
    if (handle->use_colors)
    {
        switch(message->log_level)
        {
            case 0:
                color = JAPML_CURSES_DEBUG_COLOR;
                break;
            case 1:
                color = JAPML_CURSES_INFO_COLOR;
                break;
            case 2:
                color = JAPML_CURSES_ERROR_COLOR;
                break;
            case 3:
                color = JAPML_CURSES_CRITICAL_COLOR;
                break;
        }

        attron(COLOR_PAIR(color));
    }

    wprintw(handle->log_window, "%s\n", message->message);
    wmove(handle->log_window, getcury(handle->log_window), 1);

    if (handle->use_colors)
    {
        attroff(COLOR_PAIR(color));
    }
}
