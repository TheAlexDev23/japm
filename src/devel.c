#include <unistd.h>

#include <libjapml/japml.h>
#include <libjapml/handle.h>
#include <libjapml/log.h>
#include <libjapml/exit.h>
#include <libjapml/error.h>
#include <libjapml/init.h>

void test_japml()
{
    japml_handle_t* handle = japml_init_devel();
    japml_log(handle, Debug, "This is a dbug log");
    japml_log(handle, Information, "This is a info log");
    japml_log(handle, Error, "This is a error log");
    japml_log(handle, Critical, "This is a critical log");

    japml_ncurses_pb_set_lim(handle, 10);

    for (int i = 0; i < 10; i++)
    {
        japml_ncurses_pb_add(handle, 1);
        usleep(100000);
    }

    japml_throw_error(handle, malloc_error, NULL);
	
    getch();

    japml_exit(handle);
}