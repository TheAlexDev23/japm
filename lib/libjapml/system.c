#include <stdlib.h>

#include "system.h"
#include "japml.h"
#include "list.h"
#include "package.h"
#include "helper.h"

int japml_run_instructions(japml_list_t* instructions, char* dir)
{
    while (instructions)
    {
        char *instruction = (char*)(instructions->data);
        char* cmd = japml_str_replace(instruction, JAPML_PACKAGE_DIR_VAR, dir);
        if (system(cmd) == -1)
        {
            return -1;
        }

        free(cmd);

        instructions = japml_list_next(instructions);
    }

    return 0;
}