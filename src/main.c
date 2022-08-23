#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "errors.h"
#include "install.h"
#include "main.h"
#include "remove.h"
#include "update.h"
#include "IO/term.h"
#include "search.h"

#include "../lib/libjapml/japml.h"
#include "../lib/libjapml/log.h"
#include "../lib/libjapml/exit.h"

// In the early stages of development i will use this to check if japml works
void check_japml()
{
    japml_handle_t handle;

    handle.log_files = NULL;
    handle.error_log_files = NULL;

    handle.log_level = japml_log_level_t.Debug;
    handle.use_colors = true;
    handle.use_curses = true;

    terminal_init(&handle);

    japml_log(&handle, japml_log_level_t.Debug, "This is a dbug log");
    japml_log(&handle, japml_log_level_t.Information, "This is a info log");
    japml_log(&handle, japml_log_level_t.Error, "This is a error log");
    japml_log(&handle, japml_log_level_t.Critical, "This is a critical log");

    sleep(5);

    exit_japml(&handle);
}

int main(int argc, char **argv)
{
    check_japml();

    if (check_root())
        return 2;

    if (check_usage(argc, argv))
        return 1;

    int input_type = check_input(argc, argv);

    switch (input_type)
    {
		case 1:
			if (check_install_usage(argc, argv)) exit(wrong_usage_error);
			
			for (int i = 0; i < argc - 2; i++) 
			{
				install_single_package(argv[i + 2]);
			}
			break;
		case 2:
			if (check_remove_usage(argc, argv)) exit(wrong_usage_error);
			
			if (strcmp(argv[2], "-r") == 0)
			{
				for (int i = 0; i < argc - 3; i++) 
				{
					remove_package_rec(argv[i + 3]);
				}
			}
			else
			{
				for (int i = 0; i < argc - 2; i++) 
				{
					remove_package(argv[i + 2]);
				}
			}

			break;
		case 3:
			if (check_update_usage(argc, argv)) exit(wrong_usage_error);

			for (int i = 0; i < argc - 2; i++) 
			{
				if (strcmp(argv[i + 2], "system") == 0) update_system();
				update_package(argv[i + 2]);
			}
			break;
		case 4:
			search(argv[2]);
			break;
		default:
			fprintf(stderr, "Something went wrong, operation not recognized\n");
    }

    return 0;
}

int check_input(int argc, char **argv)
{
    if (strcmp(argv[1], "install") == 0)
        return 1;
    if (strcmp(argv[1], "remove") == 0)
        return 2;
    if (strcmp(argv[1], "update") == 0)
        return 3;
    if (strcmp(argv[1], "search") == 0)
        return 4;
}
