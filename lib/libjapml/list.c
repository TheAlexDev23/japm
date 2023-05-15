#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

    // The list already has nodes so go to the last node
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
    if (list == NULL)
    {
        return 0;
    }
    
    int i = 1;
    while ((list = japml_list_next(list)) != NULL)
    {
        i++;
    }

    return i;
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

void japml_list_free_string(japml_list_t* list)
{
    japml_list_t* it = list;
    while (it != NULL)
    {
        free(it->data);
        it = japml_list_next(it);
    }

    japml_list_free(list);
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
    japml_list_t* list = NULL;

    char* token = strtok(string_list, ";");
    while (token)
    {
        char* value = malloc(strlen(token) + 1);
        strcpy(value, token);
        japml_list_add(handle, &list, value);
        token = strtok(NULL, ";");
    }

    return list;
}

char* japml_list_to_string(japml_handle_t* handle, japml_list_t* list)
{
    char* string_list = calloc(MAX_CHAR_LIST_LENGTH + 1, sizeof(char));
    while (list)
    {
        strcat(string_list, (char*)(list->data));
        strcat(string_list, ";");

        list = japml_list_next(list);
    }

    return string_list;
}