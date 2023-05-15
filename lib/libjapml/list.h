#ifndef _JAPML_LIST_H_INCLUDED
#define _JAPML_LIST_H_INCLUDED

#include "japml.h"

struct _japml_list
{
	void *data;
	japml_list_t *next;
};

/* 
* Creates an empty list of size 'size'
* Returns pointer to list
*/
japml_list_t* japml_list_create_empty(japml_handle_t* handle, int size);

/* Frees list 'list' */
void japml_list_free(japml_list_t* list);
/* Frees list 'list' and the value pointed by data on each element. */
void japml_list_free_string(japml_list_t* list);

/* Adds a new node to the list. */
void japml_list_add(japml_handle_t* handle, japml_list_t** list, void* data);

/* Returns the amount of items in list */
int japml_list_length(japml_list_t* list);

/* 
* Returns the node after 'list' in the list
* Returns NULL if 'list' is NULL or if no node is after 'list'
*/
japml_list_t* japml_list_next(japml_list_t *list);

/* 
* Returns the last node in the 'list' list
* Returns NULL if list is empty
*/
japml_list_t* japml_list_last(japml_list_t *list);

/* 
* Returns element 'n' from list 'list'
* Returns NULL if list is empty or if the list is smaller than 'n'
*/
japml_list_t* japml_list_get_element(japml_list_t *list, int n);

#define MAX_CHAR_LIST_LENGTH 5000

/* string_list is converted into a jaml list */
japml_list_t* japml_string_to_list(japml_handle_t* handle, char* string_list);
/* Retuns a string with all items of a list each sepparated by a semicolon */
char* japml_list_to_string(japml_handle_t* handle, japml_list_t* list);

#endif