#include <string.h>
#include <stdbool.h>

#include "japml.h"
#include "parser.h"
#include "package.h"
#include "input.h"
#include "list.h"
#include "file.h"

void japml_print_program_usage(int argc, char** argv)
{
    printf("Usage: %s [options] <action> {packages} \n", argv[0]);
    printf("\nPackage management: \n");
    printf("    Install package: " JAPML_INSTALL_ARG " {packages}\n");
    printf("    Remove package: " JAPML_REMOVE_ARG " {packages}\n");
    printf("    Update package: " JAPML_UPDATE_ARG " {packages}\n");
    printf("    Search for package: " JAPML_SEARCH_ARG " {packages}\n");

    printf("\nFlags and parameters\n");

    printf("\nGeneral:\n");
    printf("    Developer mode: " JAPML_DEVEL_ARG "\n");
    printf("    Default to all: " JAPML_DEFAULT_TO_ALL_ARG "\n");
    printf("    Don't quit upon critical errors: " JAPML_STAY_ON_CRITICAL_ARG "\n");
    printf("    Remove a package recursively: " JAPML_REMOVE_RECURSIVE_ARG "\n");

    printf("\nLogging:\n");
    printf("    Modify log level:" JAPML_LOG_LEVEL_ARG " [level]\n");
    printf("    Provide log files: " JAPML_LOG_FILES_ARG " {log_files}\n");
    printf("    Provide error log files: " JAPML_ERROR_LOG_FILES_ARG " {error_log_files}\n");

    printf("\nNcurses/TUI:\n");
    printf("    Disable ncurses mode: " JAPML_NO_CURSES_ARG "\n");
    printf("    Disable color: " JAPML_NO_COLOR_ARG "\n");
}

bool japml_check_input(japml_parse_parameters_t* params)
{
    if (!params->package_action)
    {
        return false;
    }

    if (params->remove_recursive)
    {
        if (*(params->package_action) != japml_package_remove && *(params->remove_recursive) == false)
        {
            return false;
        }
    }

    return true;
}

japml_parse_parameters_t* japml_create_parse_params()
{
    japml_parse_parameters_t* params = malloc(sizeof(japml_parse_parameters_t));

    params->wrong_param =
    params->devel =
    params->default_to_all =
    params->exit_on_critical = NULL;

    params->packages = NULL;
    params->package_action = NULL;
    params->remove_recursive = NULL;

    params->log_level = NULL;

    params->log_files = 
    params->error_log_files = NULL;

    params->curses =
    params->color = NULL;

    return params;
}

japml_parse_parameters_t* japml_parse_input(int argc, char** argv)
{
    // Shortcut if we want to print usage
    if (argc == 2 && strcmp(argv[1], JAPML_HELP_ARG) == 0)
    {
        japml_print_program_usage(argc, argv);
        return NULL;
    }

    japml_parse_parameters_t* params = japml_create_parse_params();

    params->devel = malloc(sizeof(bool));
    *(params->devel) = false;

    for (int i = 1; i < argc; i++)
    {
        if (japml_parse_arg(i, argc, argv, params))
        {
            params->wrong_param = malloc(sizeof(bool));
            *(params->wrong_param) = true;
            return params;
        }
    }

    if (!japml_check_input(params))
    {
        return NULL;
    }

    return params;
}

int japml_parse_arg(int count, int argc, char** argv, japml_parse_parameters_t* params)
{
    char* arg = argv[count];
    japml_package_action_t* action_type = malloc(sizeof(japml_package_action_t));

    if (japml_is_action(arg, action_type))
    {
        params->package_action = action_type;
        params->packages = japml_get_param_list(count, argc, argv);
        return 0;
    }

    if (!japml_input_is_param(arg))
    {
        return 0;
    }

    japml_list_t* char_params = japml_get_param_list(count, argc, argv);

    if (strcmp(arg, JAPML_DEVEL_ARG) == 0)
    {
        *(params->devel) = true;
    }
    else if (strcmp(arg, JAPML_DEFAULT_TO_ALL_ARG) == 0)
    {
        params->default_to_all = malloc(sizeof(bool));
        *(params->default_to_all) = true;
    }
    else if (strcmp(arg, JAPML_STAY_ON_CRITICAL_ARG) == 0)
    {
        params->exit_on_critical = malloc(sizeof(bool));
        *(params->exit_on_critical) = false;
    }
    else if (strcmp(arg, JAPML_REMOVE_RECURSIVE_ARG) == 0)
    {
        params->remove_recursive = malloc(sizeof(bool));
        *(params->remove_recursive) = true;
    }
    else if (strcmp(arg, JAPML_LOG_LEVEL_ARG) == 0)
    {
        if (japml_list_length(char_params) != 1)
        { goto break_if; }

        char* param = (char*)(char_params->data);

        japml_log_level_t log_level;
        if (strcmp(param, "Debug") == 0)
        {
            log_level = Debug;
        }
        else if (strcmp(param, "Information") == 0)
        {
            log_level = Information;
        }
        else if (strcmp(param, "Error") == 0)
        {
            log_level = Error;
        }

        params->log_level = malloc(sizeof(japml_package_action_t));
        *(params->log_level) = log_level;
    }
    else if (strcmp(arg, JAPML_LOG_FILES_ARG) == 0)
    {
        params->log_files = japml_create_file_list(char_params);
    }
    else if (strcmp(arg, JAPML_ERROR_LOG_FILES_ARG) == 0)
    {
        params->error_log_files = japml_create_file_list(char_params);
    }
    else if (strcmp(arg, JAPML_NO_CURSES_ARG) == 0)
    {
        params->curses = malloc(sizeof(bool));
        *(params->curses) = false;
    }
    else if (strcmp(arg, JAPML_NO_COLOR_ARG) == 0)
    {
        params->color = malloc(sizeof(bool));
        *(params->color) = false;
    }
    else
    {
        return 1;
    }

    break_if:
    japml_list_free(char_params);
    return 0;
}

japml_list_t* japml_get_param_list(int count, int argc, char** argv)
{
    japml_list_t* list = NULL;
    while (!japml_input_is_param(argv[count + 1]))
    {
        char* param = malloc(sizeof(char) * (strlen(argv[count + 1]) + 1));
        strcpy(param, argv[count + 1]);
        japml_list_add(NULL, &list, param);
        count++;
        if (count >= argc - 1)
        {
            break;
        }
    }

    return list;
}

bool japml_input_is_param(char* arg)
{ return (arg[0] == '-' || japml_is_action(arg, NULL)); }

bool japml_is_action(char* arg, japml_package_action_t* type)
{
    if (strcmp(arg, "install") == 0)
    {
        if (type)
        {
            *type = japml_package_install;
        }
        return true;
    }
    else if (strcmp(arg, "remove") == 0)
    {
        if (type)
        {
            *type = japml_package_remove;
        }
        return true;
    }
    else if (strcmp(arg, "update") == 0)
    {
        if (type)
        {
            *type = japml_package_update;
        }
        return true;
    }
    else if (strcmp(arg, "search") == 0)
    {
        if (type)
        {
            *type = japml_package_search;
        }
        return true;
    }
    else
    {
        return false;
    }
}

void japml_free_parse_params(japml_parse_parameters_t* params)
{
    free(params->wrong_param);
    free(params->devel);
    free(params->default_to_all);
    free(params->exit_on_critical);

    japml_free_string_list(params->packages);
    free(params->package_action);
    free(params->remove_recursive);

    free(params->log_level);

    /* 
    * Lists of files are referenced by the handle, and are both closed and freed upon exit.
    * Therefore there's no need to free and/or close log_files and error_log_files
    */

   free(params->curses);
   free(params->color);
}