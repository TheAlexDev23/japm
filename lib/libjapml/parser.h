#ifndef _PARSER_H_INCLUDED
#define _PARSER_H_INCLUDED

#include <stdbool.h>

#include "japml.h"
#include "log.h"
#include "list.h"
#include "input.h"

/* A struct represeting comand line arguments */
struct _japml_parse_parameters
{
    bool wrong_param;
    // * General
    
    bool devel;
    bool default_to_all;
    bool exit_on_critical;

    // * Logging
    japml_log_level_t log_level;
    japml_list_t* log_files;
    japml_list_t* error_log_files;
    
    // * Curses
    bool curses;
    bool color;
};

/* Creates a parsed parameter strcuture according to the arguments */
japml_parse_parameters_t* japml_parse_input(int argc, char** argv);

/* Returns true if arg is a flag or named parameter false otherwise */
bool japml_input_is_param(char* arg);

/* Parses argument at argc */
int japml_parse_arg(int argc, char** argv, japml_parse_parameters_t* params);

/* Gets all the parameters until the next named parameter and returns a list */
japml_list_t* japml_get_param_list(int argc, char** argv);

#endif