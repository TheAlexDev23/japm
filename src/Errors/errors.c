#include <stdio.h>
#include <string.h>

//#include "errors.h"

#define ACCEPTED_ARGS_LENGTH 5

char* accepted_args[ACCEPTED_ARGS_LENGTH] = {"install", "remove", "update", "upgrade", "search"};

void 
usage(char*program_name) 
{
    printf("Usage:\n");
    printf("    %s [OPTIONS]\n", program_name);
}

void 
bad_usage(char*arg, char*program_name) 
{
    printf("Wrong Argument %s\n\n", arg);
    usage(program_name);
}

void 
check_usage(int argc, char**argv) 
{
    if (argc == 1) {
        usage(argv[0]);
        return;
    }

    for (int i = 1; i < argc; i++) {
        for (int j = 0; j < ACCEPTED_ARGS_LENGTH; j++) {
            if (strcmp(argv[i], accepted_args[j]) == 0) {
                goto exit_loop;
            }
        }

        bad_usage(argv[i], argv[0]);
        return;

        exit_loop:
    }
}
