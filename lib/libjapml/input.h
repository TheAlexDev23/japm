#ifndef _JAPML_INPUT_H_INCLUDED
#define _JAPML_INPUT_H_INCLUDED

enum _japml_arg_type {
    japml_arg_devel,
    japml_arg_defaul_to_all,
    japml_arg_stay_on_crit,
    japml_arg_log_level,
    japml_arg_log_files,
    japml_arg_error_log_files,
    japml_arg_curses,
    japml_arg_no_color
};

#define JAPML_INSTALL_ARG "install"
#define JAPML_REMOVE_ARG "remove"
#define JAPML_UPDATE_ARG "update"
#define JAPML_SEARCH_ARG "search"

#define JAPML_DEVEL_ARG "-d"
#define JAPML_DEFAULT_TO_ALL_ARG "-Y"
#define JAPML_STAY_ON_CRITICAL_ARG "-critstay"

#define JAPML_LOG_LEVEL_ARG "--log_level"
#define JAPML_LOG_FILES_ARG "--log_files"
#define JAPML_ERROR_LOG_FILES_ARG "--error_log_files"

#define JAPML_NO_CURSES_ARG "-nocurses"
#define JAPML_NO_COLOR_ARG "-nocolor"

#endif