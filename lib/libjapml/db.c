#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#include "db.h"
#include "japml.h"
#include "package.h"
#include "log.h"
#include "error.h"
#include "list.h"
#include "json.h"
#include "file.h" // japml_create_file_recursive
#include "japmlcurses.h" // japml_ncurses_pl_add
#include "internet.h"

// Creates local.db and the packages table
void japml_create_local_db(japml_handle_t* handle)
{
    japml_log(handle, Information, "Creating local database ...");
    japml_create_file_recursive("/var/japml/local.db");
    sqlite3_open("/var/japml/local.db", &handle->sqlite);
    sqlite3_exec(handle->sqlite,
                 "CREATE TABLE packages ( \
                 \"name\" TEXT NOT NULL, \
                 \"description\" TEXT, \
                 \"version\" TEXT NOT NULL, \
                 \"remove\" TEXT NOT NULL);",
                 NULL, NULL, NULL);
}

int callback(void *ptr, int column_num, char **values, char **rows)
{
    char *temp_file = "/tmp/japml/sql_callback_temp_file";
    japml_create_file_recursive(temp_file);
    // Basically empties the file
    fclose(fopen(temp_file, "w"));

    FILE *fp = fopen(temp_file, "a");

    for (int i = 0; i < column_num; i++)
    {
        fprintf(fp, "%s\n", values[i]);
    }

    fclose(fp);
    return 0;
}

japml_package_t* japml_get_package_from_local_db(japml_handle_t* handle, char* package_name)
{
    char *sql = malloc(sizeof(char) *
        (strlen("SELECT * FROM packages WHERE name = ''") 
        + strlen(package_name) + strlen(";") + 1));

    sprintf(sql, "SELECT * FROM packages WHERE name ='%s';", package_name);

    remove("/tmp/japml/sql_callback_temp_file");
    
    char* errMsg = 0;
    if (sqlite3_exec(handle->sqlite, sql, callback, NULL, &errMsg))
    {
        sprintf(handle->log_message, "Cannot execute sql (%s)", errMsg);
        japml_throw_error(handle, sql_command_fail_error, handle->log_message);
        return NULL;
    }

    free(sql);

    // Absence of callback file means such package was not found in db
    if (access("/tmp/japml/sql_callback_temp_file", F_OK) != 0) 
    {
        return NULL;
    }

    FILE *fp = fopen("/tmp/japml/sql_callback_temp_file", "r");

    if (!fp)
    {
        japml_throw_error(handle, custom_error_error, "Cannot open sql callback temporary file");
        return NULL;
    }

    japml_package_t* package = japml_create_empty_package();

    char** fields_to_modif[3] = {
        &package->name,
        &package->description,
        &package->version
    };

    int i = 0;
    char line[5000];
    while(fgets(line, sizeof(line), fp) && i <= 2)
    {
        // Remove newline character
        line[strlen(line) - 1] = '\0';
        char* val = malloc(strlen(line) + 1);
        strcpy(val, line);
        *(fields_to_modif[i]) = val;
        i++;
    }

    package->remove = japml_string_to_list(handle, line);

    return package;
}

japml_package_t* japml_get_package_from_remote_db(japml_handle_t* handle, char* package_name)
{
    if (!handle->curl) {
        japml_throw_error(handle, custom_error_critical, "No instance of cURL found on handle");
    }

    japml_list_t* remote_dbs = handle->remote_dbs;

    bool found = false;
    while (remote_dbs)
    {
        char* remote_url = ((japml_db_remote_t*)remote_dbs->data)->url;
        char* url = malloc(sizeof(char) * (strlen(remote_url) + strlen(package_name) + strlen("/package.json")) + 1);
        sprintf(url, "%s%s/package.json", remote_url, package_name);

        sprintf(handle->log_message, "Fetching %s", url);
        japml_log(handle, Information, handle->log_message);

        int res = japml_web_download_file(handle, url, "/tmp/japml/packagefetch");
        free(url);

        if (res)
        {
            japml_log(handle, Information, "Not found");
        }
        else
        {
            japml_log(handle, Information, "Found");
            found = true;
            break;
        }

        free(url);

        remote_dbs = japml_list_next(remote_dbs);
    }

    if (!found)
    {
        japml_throw_error(handle, package_not_found_error, "Package cannot be found in any remote database");
        return NULL;
    }

    japml_package_t* package = japml_parse_json_file(handle, "/tmp/japml/packagefetch");
    japml_ncurses_pl_add(handle, package, japml_package_search);

    return package;
}

int japml_add_package_to_local_db(japml_handle_t* handle, japml_package_t* package)
{
    char* remove = japml_list_to_string(handle, package->remove);
    char *sql = malloc(sizeof(char) * (
        strlen("INSERT INTO packages (name, description, version, remove) VALUES ();") + 
        
            strlen(package->name) + strlen("'', ") +
            strlen(package->description) + strlen("'', ") +
            strlen(package->version) + strlen("'', ") +
            strlen(remove) + strlen("'', ") + 1)
    );

    sprintf(sql, "INSERT INTO packages (name, description, version, remove) VALUES ('%s', '%s', '%s', '%s');",
            package->name, package->description, package->version, remove);

    char *errMsg = 0;
    if (sqlite3_exec(handle->sqlite, sql, NULL, NULL, &errMsg))
    {
        sprintf(handle->log_message, "Cannot execute sql (%s)", errMsg);
        japml_throw_error(handle, sql_command_fail_error, handle->log_message);
        return 1;
    }

    free(sql);
    free(remove);
    return 0;
}

int japml_remove_package_from_local_db(japml_handle_t* handle, japml_package_t* package)
{
    char *sql = malloc(strlen("DELETE FROM packages WHERE name = '';") + strlen(package->name) + 1);

    sprintf(sql, "DELETE FROM packages WHERE name = '%s';", package->name);

    char* errMsg = 0;
    if (sqlite3_exec(handle->sqlite, sql, NULL, NULL, &errMsg))
    {
        sprintf(handle->log_message, "Cannot execute sql (%s)", errMsg);
        japml_throw_error(handle, sql_command_fail_error, handle->log_message);
        return 1;
    }

    return 0;
}