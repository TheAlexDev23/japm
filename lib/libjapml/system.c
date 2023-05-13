#include <stdlib.h>

#include "system.h"
#include "japml.h"
#include "list.h"

int japml_run_instructions(japml_list_t* instructions)
{
    while (instructions)
    {
        char *cmd = (char*)(instructions->data);
        if (system(cmd) == -1)
        {
            return -1;
        }

        instructions = japml_list_next(instructions);
    }

    return 0;
}