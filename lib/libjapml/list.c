#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "japml.h"
#include "log.h"
#include "error.h"

void japml_list_add(japml_handle_t *handle, japml_list_t **list, void* data)
{
    japml_list_t* node = malloc(sizeof(japml_list_t));
    if (!node)
    {
        char error_message[60]; // I dont't know how big the message would be
        sprintf(error_message, "Could not allocate memory of size %zu\n", sizeof(japml_list_t));
        japml_throw_error(handle, malloc_error, error_message);
    }

    node->data = data;
    node->next = NULL;

    // the list already has nodes so go to the last node
    if (*list)
    {
        japml_list_t *last_node = japml_list_last(*list);
        last_node->next = node;
    }
    else
    {
        *list = node;
    }
}

japml_list_t* japml_list_last(japml_list_t *list)
{
    japml_list_t *it = list;
    while (it->next)
    {
        it = japml_list_next(it);
    }

    return it;
}

japml_list_t* japml_list_next(japml_list_t *list)
{
    if (list)
    {
        return list->next;
    }
    
    return NULL;
}

japml_list_t* japml_list_get_element(japml_list_t *list, int n)
{
    japml_list_t* it = list;
    for (int i = 0; i < n && it; i++)
    {
        it = japml_list_next(it);
    }

    return it;
}

japml_list_t* japml_list_create_empty(japml_handle_t* handle, int size)
{
    japml_list_t *list = malloc(sizeof(japml_list_t));
    
    if (!list)
    {
        char error_message[60]; // I dont't know how big the message would be
        sprintf(error_message, "Could not allocate memory of size %zu\n", sizeof(japml_list_t));
        japml_throw_error(handle, malloc_error, error_message);
    }

    for (int i = 0; i < size; i++)
    {
        japml_list_add(handle, &list, NULL);
    }

    return list;
}

void japml_list_free(japml_list_t* list)
{
    while (list)
    {
        japml_list_t *tmp = japml_list_next(list);
        free(list);
        list = tmp;
    }
}