#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // sleep

#include "errors.h"
#include "install.h"
#include "main.h"
#include "remove.h"
#include "update.h"
#include "IO/term.h"
#include "search.h"

#include <libjapml/japml.h>
#include <libjapml/handle.h>
#include <libjapml/log.h>
#include <libjapml/exit.h>
#include <libjapml/japmlcurses.h>
#include <libjapml/error.h>

// In the early stages of development i will use this to check if japml works
void check_japml()
{
    japml_handle_t *handle = malloc(sizeof(japml_handle_t));

    // Should be enough
    handle->log_message = malloc(sizeof(10000));

    handle->error_callbacks.cb_japml_dependency_break_error = NULL;
    handle->error_callbacks.cb_japml_malloc_error = NULL;
    handle->error_callbacks.cb_japml_install_error = NULL;
    handle->error_callbacks.cb_japml_unkown_error = NULL;

    handle->log_files = NULL;
    handle->error_log_files = NULL;

    handle->log_level = Debug;
    handle->use_colors = true;
    handle->use_curses = true;

    handle->ncurses_log_buffer = NULL;
    handle->ncurses_log_buffer_count = 0;
    handle->ncurses_log_buffer_length = 0;

    terminal_init(handle);
    japml_log(handle, Debug, "This is a dbug log");
    japml_log(handle, Information, "This is a info log");
    japml_log(handle, Error, "This is a error log");
    japml_log(handle, Critical, "This is a critical log");
    japml_throw_error(handle, malloc_error, NULL);

    /*
    for (int i = 0; i < 80; i++)
    {
        char msg[10];
        sprintf(msg, "I is: %i", i);
        japml_log(handle, Information, msg);
        usleep(80000);
    }
    */

    getch();

    exit_japml(handle);
}

int main(int argc, char **argv)
{
    check_japml();

    /*

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
    */

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
