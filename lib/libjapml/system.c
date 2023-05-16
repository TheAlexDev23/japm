#include <stdlib.h>
#include <string.h>

#include "system.h"
#include "japml.h"
#include "list.h"
#include "package.h"
#include "helper.h"
#include "handle.h"
#include "log.h"

int japml_run_single_instruction(char* instruction, char* dir)
{
    char* cmd1 = japml_str_replace(instruction, JAPML_PACKAGE_DIR_VAR "/", dir);
    char* cmd2 = japml_str_replace(cmd1, JAPML_PACKAGE_DIR_VAR, dir);

    free(cmd1);

    char* surpressed_cmd = malloc(strlen(cmd2) + strlen(" 1>>  2>> ") 
        + strlen(JAPML_SYSTEM_OUT_STDIO) + strlen(JAPML_SYSTEM_OUT_ERR) + 1);

    sprintf(surpressed_cmd, "%s 1>> " JAPML_SYSTEM_OUT_STDIO " 2>> " JAPML_SYSTEM_OUT_ERR, cmd2);

    free(cmd2);

    if (system(surpressed_cmd) == -1)
    {
        return -1;
    }

    free(surpressed_cmd);

    return 0;
}

int japml_run_instructions_silent(japml_list_t* instructions, char* dir)
{
    while (instructions)
    {
        char *instruction = (char*)(instructions->data);

        if (japml_run_single_instruction(instruction, dir))
        {
            return -1;
        }

        instructions = japml_list_next(instructions);
    }

    return 0;
}

int japml_run_instructions_with_log(japml_handle_t* handle, japml_list_t* instructions, char* dir)
{
    while (instructions)
    {
        char *instruction = (char*)(instructions->data);

        if (japml_run_single_instruction(instruction, dir))
        {
            return -1;
        }

        FILE *fp = fopen(JAPML_SYSTEM_OUT_STDIO, "r");

        if (fp == NULL)
        {
            fclose(fp);
            goto skip_log;
        }

        char buffer[JAPML_SYSTEM_OUT_LOG_MAX];
        fread(buffer, JAPML_SYSTEM_OUT_LOG_MAX, 1, fp);

        japml_log(handle, Information, buffer);

        skip_log:
        instructions = japml_list_next(instructions);
    }
}