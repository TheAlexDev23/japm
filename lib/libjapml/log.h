#ifndef _LOG_H_INCLUDED
#define _LOG_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "japml.h"

enum _japml_log_level
{
    Debug = 0,
    Information = 1,
    Error = 2,
    Critical = 3
};

struct _japml_log_message
{
    char *message;
    japml_log_level_t log_level;
};

void terminal_init(japml_handle_t* handle);

void japml_log(japml_handle_t* handle, japml_log_level_t log_level, char *message);

void japml_log_normal_files(japml_handle_t* handle, japml_log_level_t log_level, char *message);
void japml_log_error_files(japml_handle_t* handle, japml_log_level_t log_level, char *message);

void japml_debug_log(FILE *output, char *message, bool color);
void japml_info_log(FILE *output, char *message, bool color);
void japml_error_log(FILE *output, char *message, bool color);
void japml_critical_log(FILE *output, char *message, bool color);

void color_reset(FILE *output);

#endif
