#include <curl/curl.h>
#include <stdio.h>

#include "internet.h"
#include "japml.h"
#include "handle.h"
#include "file.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int japml_web_download_file(japml_handle_t* handle, char* url, char* file_location)
{
    japml_create_file_recursive(file_location);
    FILE* f = fopen(file_location, "w");
    if (f == NULL)
    {
        japml_throw_error(handle, custom_error_error, "Cannot open file to write web response.");
        return -1;
    }

    curl_easy_setopt(handle->curl, CURLOPT_URL, url);
    curl_easy_setopt(handle->curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle->curl, CURLOPT_WRITEDATA, f);
    
    int res = curl_easy_perform(handle->curl);
    
    fclose(f);

    if (res != 0)
    {
        japml_throw_error(handle, unknown_error, "Unkown error. cURL action performed with unsuccesful output");
    }

    int http_code = 0;
    curl_easy_getinfo(handle->curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code == 200)
    {
        return 0;
    }
    
    return -1;
}