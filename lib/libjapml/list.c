#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strtok for list manipulation

#include "list.h"
#include "japml.h"
#include "log.h"
#include "error.h"

void japml_list_add(japml_handle_t *handle, japml_list_t **list, void* data)
{
    japml_list_t* node = malloc(sizeof(japml_list_t));
    if (!node)
    {
        if (handle)
        {
            japml_throw_error(handle, malloc_error, NULL);
        }
        return;
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

int japml_list_length(japml_list_t* list)
{
    if (!list)
    {
        return 0;
    }
    
    int i = 0;
    while ((list = japml_list_next(list)) != NULL)
    {
        i++;
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
    japml_list_t *list = NULL;
    
    if (!list)
    {
        japml_throw_error(handle, malloc_error, NULL);
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

japml_list_t* japml_string_to_list(japml_handle_t* handle, char* string_list) 
{
    japml_list_t* list;
    japml_list_add(handle, &list, string_list);

    char* token = strtok(string_list, ";");
    while (token)
    {
        japml_list_add(handle, &list, token);
        token = strtok(NULL, ";");
    }

    return list;
}

char* japml_list_to_string(japml_handle_t* handle, japml_list_t* list)
{
    char* string_list = calloc(sizeof(MAX_CHAR_LIST_LENGTH), sizeof(char));
    while (list)
    {
        strcat(string_list, (char*)(list->data));
        strcat(string_list, ";");
        list = japml_list_next(list);
    }

    return string_list;
}