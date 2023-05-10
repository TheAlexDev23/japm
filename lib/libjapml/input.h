#ifndef _INPUT_H_INCLUDED
#define _INPUT_H_INCLUDED

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

#define JAPML_DEVEL_ARG "-d"
#define JAPML_DEFAULT_TO_ALL_ARG "-Y"
#define JAPML_STAY_ON_CRITICAL_ARG "-critstay"

#define JAPML_LOG_LEVEL_ARG "--log_level"
#define JAPML_LOG_FILES_ARG "--log_files"
#define JAPML_ERROR_LOG_FILES_ARG "--error_log_files"

#define JAPML_NO_CURSES_ARG "-nocurses"
#define JAPML_NO_COLOR_ARG "-nocolor"

#endif