#ifndef _JAPML_PARSER_H_INCLUDED
#define _JAPML_PARSER_H_INCLUDED

#include <stdbool.h>

#include "japml.h"
#include "package.h"
#include "log.h"
#include "list.h"
#include "input.h"

/* A struct represeting comand line arguments */
struct _japml_parse_parameters
{
    bool* wrong_param;
    // * General
    bool* devel;
    bool* default_to_all;
    bool* exit_on_critical;

    // * Package management
    /* A list of strings each with a package name */
    japml_list_t* packages;
    japml_package_action_t* package_action;
    bool* remove_recursive;
    bool* update_systemwide;

    // * Logging
    japml_log_level_t* log_level;
    japml_list_t* log_files;
    japml_list_t* error_log_files;
    
    // * Curses
    bool* curses;
    bool* color;
};

void japml_pars_params_free(japml_parse_parameters_t* params);

/* Prints the available options and parameters to run JAPML */
void japml_print_program_usage(char** argv);

/* Creates a parsed parameter strcture according to the arguments */
japml_parse_parameters_t* japml_parse_input(int argc, char** argv);

/* Parses argument at count */
int japml_parse_arg(int count, int argc, char** argv, japml_parse_parameters_t* params);

/* Gets all the parameters until the next named parameter and returns a list */
japml_list_t* japml_get_param_list(int count, int argc, char** argv);

/* Returns true if arg is a package action ex. install or remove. Also sets type to the type of action */
bool japml_is_action(char* arg, japml_package_action_t* type);

/* Returns true if arg is a flag or named parameter false otherwise */
bool japml_input_is_param(char* arg);

#endif