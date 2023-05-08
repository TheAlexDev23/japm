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

// TEST
// REMINDER -> maybe transfer this to a separate file
int japml_create_file_recursive(char* pathname)
{
    for (char *p = pathname; (p = strchr(p ,'/')) != NULL; ++p) {
        char c = p[1];
        p[1] = '\0';
        errno = 0;
        if (mkdir(pathname, 0700) != 0 && errno != EEXIST) {
            return -1;
        }
        p[1] = c;
    }
    int fd = creat(pathname, 0600);

    return fd;
}

// Creates local.db and the packages table
void japml_create_local_db(japml_handle_t* handle)
{
    japml_create_file_recurisve("/var/japml/local.db");
    sqlite3_open("/var/japml/local.db", &handle->sqlite);
    sqlite3_exec(handle->sqlite,
                 "CREATE TABLE packages ( \ 
                 \"name\" TEXT NOT NULL, \ 
                 \"description\" TEXT, \ 
                 \"version\" TEXT NOT NULL, \ 
                 \"remove\" TEXT);",
                 NULL, NULL, NULL);
}

int callback(void *ptr, int column_num, char **values, char **rows)
{
    char *temp_file = "/tmp/japml/sql_callback_temp_file";
    japml_create_file_recurisve(temp_file);
    // Basically empties the file
    fclsoe(fopen(temp_file, "w"));

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
        sprintf(handle->log_message, "Canont execute sql (%s)", errMsg);
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
    int i = 0;
    char line[5000];

    japml_package_t* package = malloc(sizeof(package));

    char* fields_to_modif[3] = {
        package->name,
        package->description,
        package->version
    };

    int i = 0;
    while(fgets(line, sizeof(line), fp))
    {
        // Remove newline character
        line[strlen(line) - 1] = "\0";
        fields_to_modif[i] = malloc(sizeof(char) * strlen(line));
        strcpy(fields_to_modif[i], line);
        i++;
    }

    package->remove = japml_string_to_list(handle, line);

    return package;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

japml_package_t* japml_get_package_from_remote_db(japml_handle_t* handle, char* package_name)
{
    if (!handle->curl) {
        japml_throw_error(handle, custom_error_critical, "No instance of cURL found on handle");
    }

    FILE* f = fopen("/tmp/japml/packagefetch", "w");

    japml_list_t* remote_dbs = handle->remote_dbs;

    bool found = false;
    while (remote_dbs)
    {
        sprintf(handle->log_message, "Fetching %s", ((japml_db_remote_t*)remote_dbs->data)->url);
        japml_log(handle, Information, handle->log_message);

        if (handle->curl)
        {
            curl_easy_setopt(handle->curl, CURLOPT_URL, ((japml_db_remote_t*)(remote_dbs->data))->url);
            curl_easy_setopt(handle->curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(handle->curl, CURLOPT_WRITEDATA, f);
            int res = curl_easy_perform(handle->curl);
            if (res != 0)
            {
                japml_throw_error(handle, unknown_error, "Unkown error. cURL action performed with unsuccesful output");
            }

            int http_code = 0;
            curl_easy_getinfo(handle->curl, CURLINFO_RESPONSE_CODE, &http_code);

            // Package found, no need to search other repos.
            if (http_code == 200)
            {
                japml_log(handle, Information, "Found");
                found = true;
                break;
            }

            japml_log(handle, Information, "Not found");
        }
    }

    if (!found)
    {
        japml_throw_error(handle, package_not_found_error, "Package cannot be found in any remote database");
        return NULL;
    }

    japml_package_t* package = japml_parse_json_file(handle, "/tmp/japml/packagefetch");

    return package;
}

int japml_add_package_to_local_db(japml_handle_t* handle, japml_package_t* package)
{
    char* remove = japml_list_to_string(handle, package->remove);
    char *sql = malloc(sizeof(char) *
        strlen("INSERT INTO packages (name, description, version, remove) VALUES ();") + 
        
            strlen(package->name) + strlen("'', ") +
            strlen(package->description) + strlen("'', ") +
            strlen(package->version) + strlen("'', ") +
            strlen(remove) + strlen("'', ")
    );

    sprintf(sql, "INSERT INTO packages (name, description, verson, remove) VALUES ( \
            '%s', '%s', '%s', '%s');",
            package->name, package->description, package->version, remove);

    char *errMsg = 0;
    if (sqlite3_exec(handle->sqlite, sql, NULL, NULL, &errMsg))
    {
        sprintf(handle->log_message, "Canont execute sql (%s)", errMsg);
        japml_throw_error(handle, sql_command_fail_error, handle->log_message);
        return 1;
    }

    free(sql);
    free(remove);
    return 0;
}

int japml_remove_package_from_local_db(japml_handle_t* handle, japml_package_t* package)
{
    char *sql = malloc(sizeof(char) * 
        strlen("DELETE FROM packages WHERE name = '';") +
        strlen(package->name));
    
    sprintf(sql, "DELETE FROM packages WHERE name = '%s';", package->name);

    char* errMsg = 0;
    if (sqlite3_exec(handle->sqlite, sql, NULL, NULL, &errMsg))
    {
        sprintf(handle->log_message, "Canont execute sql (%s)", errMsg);
        japml_throw_error(handle, sql_command_fail_error, handle->log_message);
        return 1;
    }

    return 0;
}