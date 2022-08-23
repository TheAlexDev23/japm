#ifndef _LIST_H_INCLUDED
#define _LIST_H_INCLUDED

#include "japml.h"

struct _japml_list
{
	void *data;
	japml_list_t *next; // If the compiler throws error just replace by _japml_list
};

void japml_list_add(japml_handle_t* handle, japml_list_t* list, void* data);
japml_list_t* japml_list_next(japml_list_t *list);
japml_list_t* japml_list_last(japml_list_t *list);
#endif