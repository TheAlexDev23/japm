#include <string.h>
#include <stdbool.h>

#include "japml.h"
#include "parser.h"
#include "package.h"
#include "input.h"
#include "list.h"
#include "file.h"

japml_parse_parameters_t* japml_parse_input(int argc, char** argv)
{
    japml_parse_parameters_t* params = malloc(sizeof(japml_parse_parameters_t));
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
{ return (arg[1] == '-' || japml_is_action(arg, NULL)); }

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