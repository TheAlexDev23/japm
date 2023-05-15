#include <ncurses.h>
#include <string.h>
#include <unistd.h> // sleep
#include <stdio.h>
#include <ctype.h> // tolower

#include "japml.h"
#include "handle.h"
#include "error.h"
#include "japmlcurses.h"
#include "log.h"
#include "package.h" // japml_package_action_t


// * Logging

void japml_ncurses_free_log_buffer(japml_handle_t* handle)
{
    // Free the malloc'd japml_log_message_t structs first
    japml_list_t* it = handle->ncurses_lb;
    while (it)
    {
        // Since we malloc'd the message we need to free it to
        free(((japml_log_message_t*)it->data)->message);
        free(it->data);
        it = japml_list_next(it);
    }

    // Free the list itself
    japml_list_free(handle->ncurses_lb);
}

void japml_ncurses_log(japml_handle_t* handle, japml_log_level_t log_level, char *message)
{
    japml_log_message_t* message_struct = malloc(sizeof(japml_log_message_t));
    
    if (!message_struct)
    {
        japml_throw_error(handle, malloc_error, NULL);
    }

    char *msg = malloc(strlen(message) + 1);
    strcpy(msg, message);

    message_struct->log_level = log_level;
    message_struct->message = msg;

    japml_list_add(handle, &handle->ncurses_lb, message_struct);

    handle->ncurses_lb_count++;

    japml_ncurses_log_win_refresh(handle);
}

void japml_ncurses_log_win_refresh(japml_handle_t *handle)
{
    wclear(handle->log_window);
    wmove(handle->log_window, 1, 1);

    int i;

    if (handle->ncurses_lb_count > handle->ncurses_lb_length)
    {
        i = handle->ncurses_lb_count - handle->ncurses_lb_length;
    }
    else { i = 0; }

    for (; i < handle->ncurses_lb_count; i++)
    {
        japml_ncurses_log_win_print(handle, (japml_log_message_t*)(japml_list_get_element(handle->ncurses_lb, i)->data));
    }

    box(handle->log_window, ACS_VLINE, ACS_HLINE);
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

        wattron(handle->log_window, COLOR_PAIR(color));
    }

    switch(message->log_level)
    {
        case 0:
            wprintw(handle->log_window, "==> DBug: ");
            break;
        case 1:
            wprintw(handle->log_window, "==> Inf: ");
            break;
        case 2:
            wprintw(handle->log_window, "==> Err: ");
            break;
        case 3:
            wprintw(handle->log_window, "==> Crit: ");
            break;
    }

    wprintw(handle->log_window, "%s\n", message->message);
    wmove(handle->log_window, getcury(handle->log_window), 1);

    if (handle->use_colors)
    {
        wattroff(handle->log_window, COLOR_PAIR(color));
    }
}

void japml_ncurses_pb_set_lim(japml_handle_t* handle, int limit)
{
    if (!handle->use_ncurses) { return; }

    if (limit < 0)
    {
        return;
    }
    
    handle->ncurses_pb_lim = limit;
    japml_ncurses_pb_refresh(handle);
}

void japml_ncurses_pb_add(japml_handle_t* handle, int amnt)
{
    if (!handle->use_ncurses) { return; }

    if (handle->ncurses_pb_progress < handle->ncurses_pb_lim)
    {
        handle->ncurses_pb_progress += amnt;
    }

    japml_ncurses_pb_refresh(handle);
}

void japml_ncurses_pb_refresh(japml_handle_t* handle)
{
    if (!handle->use_ncurses) { return; }
    if (handle->ncurses_pb_lim == 0)
    {
        japml_ncurses_draw_pb(handle, 0);
    }

    float percentage = (float)handle->ncurses_pb_progress / (float)handle->ncurses_pb_lim;
    int x, y;
    getmaxyx(handle->progress_window, y, x);

    japml_ncurses_draw_pb(handle, percentage * (float)(x - 2));
}

void japml_ncurses_draw_pb(japml_handle_t* handle, int amnt)
{
    wclear(handle->progress_window);

    if (amnt == 0)
    {
        return;
    }

    wmove(handle->progress_window, 1, 1);

    for (int i = 0; i < amnt - 1; i++)
    {
        wprintw(handle->progress_window, "-");
        wmove(handle->progress_window, getcury(handle->log_window), 1);
    }

    wmove(handle->progress_window, getcury(handle->log_window), 1);
    wprintw(handle->progress_window, ">");

    box(handle->progress_window, ACS_VLINE, ACS_HLINE);
    wrefresh(handle->progress_window);
}

void japml_ncurses_pl_add(japml_handle_t* handle, japml_package_t* package, japml_package_action_t action)
{
    if (package == NULL)
    {
        return;
    }

    if (action == japml_package_install)
    {
        char* msg = malloc(strlen(package->name) + strlen(" => installed") + 1);
        sprintf(msg, "%s => installed", package->name);
        japml_list_add(handle, &handle->ncurses_pl_buffer, msg);
    }
    else if (action == japml_package_remove)
    {
        char* msg = malloc(strlen(package->name) + strlen(" => removed") + 1);
        sprintf(msg, "%s => removed", package->name);
        japml_list_add(handle, &handle->ncurses_pl_buffer, msg);
    }
    else if (action == japml_package_search)
    {
        char* msg = malloc(strlen(package->name) + strlen(" => searched") + 1);
        sprintf(msg, "%s => searched", package->name);
        japml_list_add(handle, &handle->ncurses_pl_buffer, msg);
    }

    handle->ncurses_pl_count++;

    japml_ncurses_pl_refresh(handle);
}

void japml_ncurses_pl_refresh(japml_handle_t* handle)
{
    wclear(handle->package_list_window);
    wmove(handle->package_list_window, 1, 1);

    int i = 0;
    if (handle->ncurses_pl_count > handle->ncurses_pl_length)
    {
        i = handle->ncurses_pl_count - handle->ncurses_pl_length;
    }

    for (; i < handle->ncurses_pl_count; i++)
    {
        char* msg = (char*)(japml_list_get_element(handle->ncurses_pl_buffer, i)->data);
        wprintw(handle->package_list_window, "%s\n", msg);
        wmove(handle->package_list_window, getcury(handle->package_list_window), 1);
    }

    box(handle->package_list_window, ACS_VLINE, ACS_HLINE);
    wrefresh(handle->package_list_window);
}

bool japml_ncurses_Yn_dialogue(japml_handle_t* handle, char* message)
{
    if (handle->default_to_all)
    {
        return true;
    }

    Yn_dialogue_start_again:
    sprintf(handle->log_message, "%s [Y/n] ", message);
    japml_log(handle, Information, handle->log_message);

    char ch;
    if (handle->use_ncurses)
    {
        ch = getch();
    }
    else
    {
        char line[2];
        if (fgets(line, sizeof(line), stdin)) {
            sscanf(line, "%c", &ch);
        }
    }
    
    if (tolower(ch) == 'y' || ch == '\n')
    {
        return true;
    }
    else if (tolower(ch) == 'n')
    {
        return false;
    }
    else
    {
        goto Yn_dialogue_start_again;
    }
}

bool japml_ncurses_yN_dialogue(japml_handle_t* handle, char* message)
{
    if (handle->default_to_all)
    {
        return false;
    }

    yN_dialogue_start_again:
    sprintf(handle->log_message, "%s [y/N] ", message);
    japml_log(handle, Information, handle->log_message);

    char ch;
    if (handle->use_ncurses)
    {
        ch = getch();
    }
    else
    {
        char line[2];
        if (fgets(line, sizeof(line), stdin)) {
            sscanf(line, "%c", &ch);
        }
    }
    
    if (tolower(ch) == 'n' || ch == '\n')
    {
        return false;
    }
    else if (tolower(ch) == 'y')
    {
        return true;
    }
    else
    {
        goto yN_dialogue_start_again;
    }
}

bool japml_ncurses_yn_dialogue(japml_handle_t* handle, char* message)
{
    yN_dialogue_start_again:
    sprintf(handle->log_message, "%s [y/n] ", message);
    char ch;
    if (handle->use_ncurses)
    {
        ch = getch();
    }
    else
    {
        char line[2];
        if (fgets(line, sizeof(line), stdin)) {
            sscanf(line, "%c", &ch);
        }
    }
    
    
    if (tolower(ch) == 'y')
    {
        return true;
    }
    else if (tolower(ch) == 'n')
    {
        return false;
    }
    else
    {
        goto yN_dialogue_start_again;
    }
}

void curses_init(japml_handle_t *handle)
{
    initscr();
    
    if (handle->use_colors && has_colors())
    {
        start_color();
        use_default_colors();

        init_pair(JAPML_CURSES_DEBUG_COLOR,    COLOR_YELLOW,  -1);
        init_pair(JAPML_CURSES_INFO_COLOR,     COLOR_GREEN,   -1);
        init_pair(JAPML_CURSES_ERROR_COLOR,    COLOR_RED,     -1);
        init_pair(JAPML_CURSES_CRITICAL_COLOR, COLOR_MAGENTA, -1);

        clear();
    }

    int maxX, maxY;

    getmaxyx(stdscr, maxY, maxX);

    // Splash screen
    char *msg_japm    = "JAPM version 2.1.0";
    char *msg_japml   = "JAPML version 1.3.0";
    char *msg_creator = "By TheAlexDev23 (https://github.com/thealexdev23)";

    mvprintw(maxY / 2 - 1, (maxX-strlen(msg_japm)) / 2, "%s", msg_japm);
    mvprintw(maxY / 2, (maxX-strlen(msg_japml)) / 2, "%s", msg_japml);
    mvprintw(maxY / 2 + 1, (maxX-strlen(msg_creator)) / 2, "%s", msg_creator);

    refresh();

    sleep(1);

    clear();

    refresh();

    handle->log_window          = newwin(maxY - PROGRESS_BAR_WINDOW_HEIGHT, maxX - PACKAGE_LIST_WINDOW_WIDTH, 0, 0);
    handle->progress_window     = newwin(PROGRESS_BAR_WINDOW_HEIGHT, maxX - PACKAGE_LIST_WINDOW_WIDTH, maxY - PROGRESS_BAR_WINDOW_HEIGHT, 0);
    handle->package_list_window = newwin(maxY, PACKAGE_LIST_WINDOW_WIDTH, 0, maxX - PACKAGE_LIST_WINDOW_WIDTH);

    box(handle->log_window,          ACS_VLINE, ACS_HLINE);
    box(handle->progress_window,     ACS_VLINE, ACS_HLINE);
    box(handle->package_list_window, ACS_VLINE, ACS_HLINE);

    wrefresh(handle->log_window);
    wrefresh(handle->progress_window);
    wrefresh(handle->package_list_window);

    handle->ncurses_lb_length = getmaxy(handle->log_window) - 2;
    handle->ncurses_pl_length = getmaxy(handle->package_list_window) -2;
}