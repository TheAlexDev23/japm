#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sqlite3.h>
#include <curl/curl.h>
#include <unistd.h>

#include "parser.h"
#include "init.h"
#include "japml.h"
#include "japmlcurses.h"
#include "log.h"
#include "handle.h"

japml_handle_t* japml_init_base()
{
    japml_handle_t* handle = malloc(sizeof(japml_handle_t));
    if (!handle)
    {
        // Log system is not initialized
        fprintf(stderr, "Crit: Criticall error initializing JAPML, could not alloc enough memory for handle\n");
        return NULL;
    }

    // * DBs

    japml_db_remote_t* remotedb = malloc(sizeof(japml_db_remote_t));
    remotedb->url = "https://raw.githubusercontent.com/TheAlexDev23/japm-official-packages/main/";

    japml_list_add(handle, &handle->remote_dbs, remotedb);

    int i = -1;
    restart_sqlitedb:
    i++;
    if (sqlite3_open("/var/japml/local.db", &handle->sqlite) && !i)
    {
        japml_create_local_db(handle);
        goto restart_sqlitedb;
    }
    
    // * Logging

    handle->log_message = malloc(10000);

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
    handle->default_to_all = !handle->use_curses;

    // * Logging

    handle->ncurses_log_buffer_count  = 0;
    handle->ncurses_log_buffer_length = 0;

    japml_terminal_init(handle);

    handle->exit_on_critical = true;
    return handle;
}

japml_handle_t* japml_init_default()
{
    japml_handle_t* handle = japml_init_base();

    handle->log_level = Information;
    
    return handle;
}

japml_handle_t* japml_init_devel()
{
    japml_handle_t* handle = japml_init_base();

    handle->log_level = Debug;
    
    return handle;
}

void japml_terminal_init(japml_handle_t* handle)
{
    if (handle->use_curses)
    {
        curses_init(handle);
    }
}

japml_handle_t* japml_init(japml_parse_parameters_t* parameters)
{
    if (!parameters)
    {
        return NULL;
    }
    
    if (parameters->wrong_param)
    {
        fprintf(stderr, "Wrong parameters. Refer to --help for proper usage.");
        return NULL;
    }

    if (geteuid() != 0)
    {
        fprintf(stderr, "Please run with loot priviligies.");
        return NULL;
    }

    japml_handle_t* handle;

    if (parameters->devel == NULL)
    {
        return NULL;
    }

    if (*(parameters->devel) == true)
    {
        handle = japml_init_devel();
    }
    else
    {
        handle = japml_init_default();
    }

    if (parameters->default_to_all != NULL)
    {
        handle->default_to_all = *(parameters->default_to_all);
    }

    if (parameters->exit_on_critical != NULL)
    {
        handle->exit_on_critical = *(parameters->exit_on_critical);
    }

    if (parameters->log_level != NULL)
    {
        handle->log_level = *(parameters->log_level);
    }

    if (parameters->log_files != NULL)
    {
        handle->log_files = parameters->log_files;
    }

    if (parameters->error_log_files != NULL)
    {
        handle->error_log_files = parameters->error_log_files;
    }

    if (parameters->curses != NULL)
    {
        handle->use_curses = *(parameters->curses);
    }

    if (parameters->color != NULL)
    {
        handle->use_colors = *(parameters->color);
    }

    return handle;
}