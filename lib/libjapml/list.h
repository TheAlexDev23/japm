#ifndef _LIST_H_INCLUDED
#define _LIST_H_INCLUDED

#include "japml.h"

struct _japml_list
{
	void *data;
	japml_list_t *next; // If the compiler throws error just replace by _japml_list
};

japml_list_t* japml_list_create_empty(japml_handle_t* handle, int size);
void japml_list_free(japml_list_t* list);

void japml_list_add(japml_handle_t* handle, japml_list_t* list, void* data);
japml_list_t* japml_list_next(japml_list_t *list);
japml_list_t* japml_list_last(japml_list_t *list);
japml_list_t* japml_list_get_element(japml_list_t *list, int n);

#endif