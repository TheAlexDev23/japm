#include <stdlib.h>

#include "list.h"
#include "japml.h"
#include "log.h"
#include "error.h"

void japml_list_add(japml_handle_t *handle, japml_list_t *list, void* data)
{
    japml_list_t* node = malloc(sizeof(japml_list_t));
    if (node)
    {
        char error_message[60]; // I dont't know how big the message would be
        sprintf(error_message, "Could not allocate memory of size %zu\n", sizeof(japml_list_t));
        japml_throw_error(handle, malloc_error, error_message);
    }

    node->data = data;
    node->next = NULL;

    // Go to the last node
    list = japml_list_last(list);
    list->next = node;
}

japml_list_t* japml_list_last(japml_list_t *list)
{
    while (list->next) // is the same as list->next != NULL
    {
        list = japml_list_next(list);
    }

    return list;
}

japml_list_t* japml_list_next(japml_list_t *list)
{
    if (list)
    {
        return list->next;
    }
    
    return NULL;
}
