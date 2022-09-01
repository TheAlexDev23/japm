#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "init.h"
#include "japmlcurses.h"
#include "japml.h"
#include "log.h"
#include "handle.h"

japml_handle_t* japml_init_default(int argc, char* argv[])
{
    japml_handle_t* handle = malloc(sizeof(japml_handle_t));

    if (!handle)
    {
        // Since not even curses is initialized
        fprintf(stderr, "Crit: Criticall error initializing JAPML, could not alloc enough memory for handle\n");
        return NULL;
    }

    // * Logging

    handle->log_message = malloc(10000);
    handle->log_level = Information;

    // TODO: Parse comand line args and check if log files are provided

    // * CURL

    handle->curl = curl_easy_init();

    if (!handle->curl)
    {
        fprintf(stderr, "Err: Error initializing JAPML could not create CURL handle");
    }

    // * TUI
    handle->use_colors = true;
    // TODO: Parse comand line args and check if curses is needed
    handle->use_curses = true;
    // TODO: Parse comand line args and check if we want default to all questions
    handle->default_to_all = false;

    // * Logging

    handle->ncurses_log_buffer_count  = 0;
    handle->ncurses_log_buffer_length = 0;

    terminal_init(handle);

    handle->exit_on_critical = true;
    return handle;
}

japml_handle_t* japml_init_devel(int argc, char* argv[])
{
    japml_handle_t* handle = malloc(sizeof(japml_handle_t));

    if (!handle)
    {
        // Since not even curses is initialized
        fprintf(stderr, "Crit: Criticall error initializing JAPML, could not alloc enough memory for handle\n");
        return NULL;
    }

    // * Logging

    handle->log_message = malloc(10000);
    handle->log_level = Debug;

    // TODO: Parse comand line args and check if log files are provided

    // * CURL

    handle->curl = curl_easy_init();

    if (!handle->curl)
    {
        fprintf(stderr, "Err: Error initializing JAPML could not create CURL handle");
    }

    // * TUI
    handle->use_colors = true;
    // TODO: Parse comand line args and check if curses is needed
    handle->use_curses = true;
    // TODO: Parse comand line args and check if we want default to all questions
    handle->default_to_all = false;

    // * Logging

    handle->ncurses_log_buffer_count  = 0;
    handle->ncurses_log_buffer_length = 0;

    terminal_init(handle);

    handle->exit_on_critical = true;

    return handle;
}

void terminal_init(japml_handle_t* handle)
{
    if (handle->use_curses)
    {
        curses_init(handle);
    }
}

japml_handle_t* japml_init(int argc, char* argv[])
{
    // TODO: Parse input when input parser will be done

    // TODO: Create a jampl handle depending if were in devel mode or not
}