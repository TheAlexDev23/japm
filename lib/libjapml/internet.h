#ifndef _JAPML_INTERNET_H_INCLUDED
#define _JAPML_INTERNET_H_INCLUDED

#include "japml.h"

/* Performs web request to url and downloads file to file_location */
int japml_web_download_file(japml_handle_t* handle, char* url, char* file_location);


#endif