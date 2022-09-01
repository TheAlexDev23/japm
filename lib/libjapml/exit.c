#include <ncurses.h>
#include <stdio.h>

#include "japml.h"
#include "exit.h"
#include "handle.h"
#include "list.h"

void exit_japml(japml_handle_t *handle)
{
     // Close all normal log files
     japml_list_t *log_files = handle->log_files;
     
     while (log_files)
     {
        fclose(log_files->data);
        log_files = japml_list_next(log_files);
     }

     // Close all error log files
     japml_list_t *error_log_files = handle->error_log_files;
    
     while (log_files)
     {
        fclose(log_files->data);
        error_log_files = japml_list_next(log_files);
     }

     if (handle->use_curses)
     {
        endwin(); // Stop curses mode
     }

     japml_ncurses_free_log_buffer(handle);
     free(handle->log_message);
     curl_easy_cleanup(handle->curl);
     free(handle);
}
