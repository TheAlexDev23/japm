#ifndef _LIST_H_INCLUDED
#define _LIST_H_INCLUDED

#include "japml.h"

struct _japml_list
{
	void *data;
	japml_list_t *next; // If the compiler throws error just replace by _japml_list
};

/* 
Creates a empty list of size 'size'
Returns pointer to list
*/
japml_list_t* japml_list_create_empty(japml_handle_t* handle, int size);

/* Frees list 'list' */
void japml_list_free(japml_list_t* list);

/* Adds a new node to the list. Requires handle to log in case of error */
void japml_list_add(japml_handle_t* handle, japml_list_t** list, void* data);

/* 
Returns the node after 'list' in the list
Returns NULL if 'list' is NULL or if no node is after 'list'
*/
japml_list_t* japml_list_next(japml_list_t *list);

/* 
Returns the last node in the 'list' list
Returns NULL if list is empty
*/
japml_list_t* japml_list_last(japml_list_t *list);

/* 
Returns element 'n' from list 'list'
Returns NULL if list is  empty or if the list is smaller than 'n'
*/
japml_list_t* japml_list_get_element(japml_list_t *list, int n);

#endif