#include <curl/curl.h>
#include <stdbool.h>

#include "db.h"
#include "japml.h"
#include "package.h"
#include "log.h"
#include "error.h"
#include "json.h"

japml_package_t* japml_get_package_from_local_db(japml_handle_t* handle, char* package_name)
{
    japml_throw_error(handle, not_implemented_error, "japml_get_package_from_local_db not implemented");
    // TODO: Open local db file

    // TODO: Use sqlite3 to get package information from local db

    // TODO: Return NULL if no package found

    // TODO: Return package found
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
        return;
    }

    japml_package_t* package = japml_parse_json_file(handle, "/tmp/japml/packagefetch");

    return package;
}