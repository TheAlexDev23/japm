#include <unistd.h>

#include <libjapml/japml.h>
#include <libjapml/handle.h>
#include <libjapml/log.h>
#include <libjapml/exit.h>
#include <libjapml/error.h>
#include <libjapml/init.h>

void test_japml()
{
    japml_handle_t* handle = japml_init_devel(1, NULL);
    japml_log(handle, Debug, "This is a dbug log");
    japml_log(handle, Information, "This is a info log");
    japml_log(handle, Error, "This is a error log");
    japml_log(handle, Critical, "This is a critical log");

    for (int i = 0; i < 80; i++)
    {
        char msg[10];
        sprintf(msg, "I is: %i", i);
        japml_log(handle, Information, msg);
        usleep(80000);
    }

    japml_throw_error(handle, malloc_error, NULL);
	
    getch();

    exit_japml(handle);
}