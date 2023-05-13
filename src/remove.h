#ifndef _REMOVE_H_INCLUDED
#define _REMOVE_H_INCLUDED

#include <libjapml/japml.h>
#include <libjapml/handle.h>
#include <libjapml/package.h>

#include <stdbool.h>

int remove_packages(japml_handle_t* handle, japml_list_t* targets, bool recursive);

#endif